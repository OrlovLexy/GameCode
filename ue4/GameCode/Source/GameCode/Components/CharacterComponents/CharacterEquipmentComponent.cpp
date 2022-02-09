#include "CharacterEquipmentComponent.h"
#include "Characters/GCBaseCharacter.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "Actors/Equipment/Throwables/ThrowableItem.h"
#include "Actors/Equipment/Weapons/MeleeWeaponItem.h"
#include "Net/UnrealNetwork.h"
#include "UI/Widget/Equipment/EquipmentViewWidget.h"
#include "UI/Widget/Equipment/WeaponWheelWidget.h"

UCharacterEquipmentComponent::UCharacterEquipmentComponent()
{
	SetIsReplicatedByDefault(true);
}

void UCharacterEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterEquipmentComponent, CurrentEquippedSlot);
	DOREPLIFETIME(UCharacterEquipmentComponent, ItemsArray);
	DOREPLIFETIME(UCharacterEquipmentComponent, AmunitionArray);
}

EEquipableItemType UCharacterEquipmentComponent::GetCurrentEquipedItemType() const
{
	EEquipableItemType Result = EEquipableItemType::None;
	if (IsValid(CurrentEquippedItem))
	{
		Result = CurrentEquippedItem->GetItemType();
	}
	return Result;
}

ARangeWeaponItem* UCharacterEquipmentComponent::GetCurrentRangeWeapon() const
{
	return CurrentEquipedWeapon;
}

AMeleeWeaponItem* UCharacterEquipmentComponent::GetCurrentMeleeWeapon() const
{
	return CurrentMeleeWeapon;
}

void UCharacterEquipmentComponent::ReloadCurrentWeapon()
{
	check(IsValid(CurrentEquipedWeapon));
	int32 AvailableAmunition = GetAvailableAmunitionForCurrentWeapon();
	if (AvailableAmunition <= 0)
	{
		return;
	}

	CurrentEquipedWeapon->StartReload();
}

void UCharacterEquipmentComponent::EquipItemInSlot(EEquipmentSlots Slot)
{
	if (bIsEquipping)
	{
		return;
	}

	UnEquipCurrentItem();

	CurrentEquippedItem = ItemsArray[(uint32)Slot];
	CurrentEquipedWeapon = Cast<ARangeWeaponItem>(CurrentEquippedItem);
	CurrentThrowableItem = Cast<AThrowableItem>(CurrentEquippedItem);
	CurrentMeleeWeapon = Cast<AMeleeWeaponItem>(CurrentEquippedItem);
	if (IsValid(CurrentEquippedItem))
	{
		UAnimMontage* EquipMontage = CurrentEquippedItem->GetCharacterEquipAnimMontage();
		if (IsValid(EquipMontage))
		{
			bIsEquipping = true;
			float EquipDuration = CachedBaseCharacter->PlayAnimMontage(EquipMontage);
			GetWorld()->GetTimerManager().SetTimer(EquipTimer, this, &UCharacterEquipmentComponent::EquipAnimationFinished, EquipDuration, false);
		}
		else
		{
			AttachCurrentItemToEquippedSocket();
		}
		CurrentEquippedItem->Equip();
	}

	if (IsValid(CurrentEquipedWeapon))
	{
		OnCurrentWeaponAmmoChangedHandle = CurrentEquipedWeapon->OnAmmoChanged.AddUFunction(this, FName("OnCurrentWeaponAmmoChanged"));
		OnCurrentWeaponReloadedHandle = CurrentEquipedWeapon->OnReloadComplete.AddUFunction(this, FName("OnWeaponReloadComplete"));
		OnCurrentWeaponAmmoChanged(CurrentEquipedWeapon->GetAmmo());
	}
	
	if (OnEquppedItemChanged.IsBound())
	{
		OnEquppedItemChanged.Broadcast(CurrentEquippedItem);
	}

	CurrentEquippedSlot = Slot;
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		Server_EquipItemInSlot(CurrentEquippedSlot);
	}
}

void UCharacterEquipmentComponent::AttachCurrentItemToEquippedSocket()
{
	if (IsValid(CurrentEquippedItem))
	{
		CurrentEquippedItem->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, CurrentEquippedItem->GetEquippedSocketName());
	}
}

void UCharacterEquipmentComponent::LaunchCurrentThrowableItem()
{
	if (CurrentThrowableItem)
	{
		CurrentThrowableItem->Throw();
		bIsEquipping = false;
		EquipItemInSlot(PreviousEquipedSlot);
	}
}

void UCharacterEquipmentComponent::UnEquipCurrentItem()
{
	if (IsValid(CurrentEquippedItem))
	{
		CurrentEquippedItem->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, CurrentEquippedItem->GetUnEquippedSocketName());
		CurrentEquippedItem->UnEquip();
	}
	if (IsValid(CurrentEquipedWeapon))
	{
		CurrentEquipedWeapon->StopFire();
		CurrentEquipedWeapon->EndReload(false);
		CurrentEquipedWeapon->OnAmmoChanged.Remove(OnCurrentWeaponAmmoChangedHandle);
		CurrentEquipedWeapon->OnReloadComplete.Remove(OnCurrentWeaponReloadedHandle);
	}

	PreviousEquipedSlot = CurrentEquippedSlot;
	CurrentEquippedSlot = EEquipmentSlots::None;
}

void UCharacterEquipmentComponent::EquipNextItem()
{
	if (CachedBaseCharacter->IsPlayerControlled())
	{
		if (IsSelectingWeapon())
		{
			WeaponWheelWidget->NextSegment();
		}
		else
		{
			APlayerController* PlayerController = CachedBaseCharacter->GetController<APlayerController>();
			OpenWeaponWheel(PlayerController);
		}
		return;
	}

	uint32 CurrentSlotIndex = (uint32)CurrentEquippedSlot;
	uint32 NextSlotIndex = NextItemsArraySlotIndex(CurrentSlotIndex);
	while ((CurrentSlotIndex != NextSlotIndex && !IsValid(ItemsArray[NextSlotIndex]))
			|| IgnoreSlotsWhileSwitching.Contains((EEquipmentSlots)NextSlotIndex))
	{
		NextSlotIndex = NextItemsArraySlotIndex(NextSlotIndex);
	}
	if (CurrentSlotIndex != NextSlotIndex)
	{
		EquipItemInSlot((EEquipmentSlots)NextSlotIndex);
	}
}

void UCharacterEquipmentComponent::EquipPreviousItem()
{
	if (CachedBaseCharacter->IsPlayerControlled())
	{
		if (IsSelectingWeapon())
		{
			WeaponWheelWidget->PreviousSegment();
		}
		else
		{
			APlayerController* PlayerController = CachedBaseCharacter->GetController<APlayerController>();
			OpenWeaponWheel(PlayerController);
		}
		return;
	}

	uint32 CurrentSlotIndex = (uint32)CurrentEquippedSlot;
	uint32 PreviousSlotIndex = PreviousItemsArraySlotIndex(CurrentSlotIndex);
	while ((CurrentSlotIndex != PreviousSlotIndex && !IsValid(ItemsArray[PreviousSlotIndex]))
			|| IgnoreSlotsWhileSwitching.Contains((EEquipmentSlots)PreviousSlotIndex))
	{
		PreviousSlotIndex = PreviousItemsArraySlotIndex(PreviousSlotIndex);
	}
	if (CurrentSlotIndex != PreviousSlotIndex)
	{
		EquipItemInSlot((EEquipmentSlots)PreviousSlotIndex);
	}
}

bool UCharacterEquipmentComponent::AddEquipmentItemToSlot(const TSubclassOf<AEquipableItem> EquipableItemClass, int32 SlotIndex)
{
	if (!IsValid(EquipableItemClass))
	{
		return false;
	}

	AEquipableItem* DefaultItemObject = EquipableItemClass->GetDefaultObject<AEquipableItem>();
	if (!DefaultItemObject->IsSlotCompatible((EEquipmentSlots)SlotIndex))
	{
		return false;
	}

	if (!IsValid(ItemsArray[SlotIndex]))
	{
		AEquipableItem* Item = GetWorld()->SpawnActor<AEquipableItem>(EquipableItemClass);
		Item->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Item->GetUnEquippedSocketName());
		Item->SetOwner(CachedBaseCharacter.Get());
		Item->UnEquip();
		ItemsArray[SlotIndex] = Item;
	}
	else if (DefaultItemObject->IsA<ARangeWeaponItem>())
	{
		ARangeWeaponItem* RangeWeaponObject = StaticCast<ARangeWeaponItem*>(DefaultItemObject);
		int32 AmmoSlotIndex = (int32)RangeWeaponObject->GetAmmoType();
		AmunitionArray[SlotIndex] += RangeWeaponObject->GetMaxAmmo();
	}

	return true;
}

void UCharacterEquipmentComponent::RemoveItemFromSlot(int32 SlotIndex)
{
	if ((uint32)CurrentEquippedSlot == SlotIndex)
	{
		UnEquipCurrentItem();
	}
	ItemsArray[SlotIndex]->Destroy();
	ItemsArray[SlotIndex] = nullptr;
}

void UCharacterEquipmentComponent::OpenViewEquipment(APlayerController* PlayerController)
{
	if (!IsValid(ViewWidget))
	{
		CreateEquipmentWidgets(PlayerController);
	}

	if (!ViewWidget->IsVisible())
	{
		ViewWidget->AddToViewport();
	}
}

void UCharacterEquipmentComponent::CloseViewEquipment()
{
	if (ViewWidget->IsVisible())
	{
		ViewWidget->RemoveFromParent();
	}
}

bool UCharacterEquipmentComponent::IsViewVisible() const
{
	bool Result = false;
	if (IsValid(ViewWidget))
	{
		Result = ViewWidget->IsVisible();
	}
	return Result;
}

void UCharacterEquipmentComponent::OpenWeaponWheel(APlayerController* PlayerController)
{
	if (!IsValid(WeaponWheelWidget))
	{
		CreateEquipmentWidgets(PlayerController);
	}

	if (!WeaponWheelWidget->IsVisible())
	{
		WeaponWheelWidget->AddToViewport();
	}
}

bool UCharacterEquipmentComponent::IsSelectingWeapon() const
{
	return IsValid(WeaponWheelWidget) && WeaponWheelWidget->IsVisible();
}

void UCharacterEquipmentComponent::ConfirmWeaponSelection() const
{
	WeaponWheelWidget->ConfirmSelection();
}

const TArray<AEquipableItem*>& UCharacterEquipmentComponent::GetItems() const
{
	return ItemsArray;
}


void UCharacterEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->IsA<AGCBaseCharacter>(), TEXT("UCharacterEquipmentComponent can be used only with AGCBaseCharacter"))
	CachedBaseCharacter = StaticCast<AGCBaseCharacter*>(GetOwner());

	CreateLoadout();
	AutoEquip();
}

void UCharacterEquipmentComponent::CreateEquipmentWidgets(APlayerController* PlayerController)
{
	checkf(IsValid(ViewWidgetClass), TEXT("UCharacterEquipmentComponent::CreateViewWidget view widget class is not defined"));

	if (!IsValid(PlayerController))
	{
		return;
	}

	ViewWidget = CreateWidget<UEquipmentViewWidget>(PlayerController, ViewWidgetClass);
	ViewWidget->InitializeEquipmentWidget(this);

	WeaponWheelWidget= CreateWidget<UWeaponWheelWidget>(PlayerController, WeaponWheelClass);
	WeaponWheelWidget->InitializeWeaponWheelWidget(this);
}

void UCharacterEquipmentComponent::Server_EquipItemInSlot_Implementation(EEquipmentSlots Slot)
{
	EquipItemInSlot(Slot);
}

void UCharacterEquipmentComponent::OnRep_ItemsArray()
{
	for (AEquipableItem* Item : ItemsArray)
	{
		if (IsValid(Item))
		{
			Item->UnEquip();
		}
	}
}

void UCharacterEquipmentComponent::CreateLoadout()
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority)
	{
		return;
	}

	AmunitionArray.AddZeroed((uint32)EAmunitionType::MAX);
	for (const TPair<EAmunitionType, int32>& AmmoPair : MaxAmunitionAmount)
	{
		AmunitionArray[(uint32)AmmoPair.Key] = FMath::Max(AmmoPair.Value, 0);
	}

	ItemsArray.AddZeroed((uint32)EEquipmentSlots::MAX);
	for (const TPair<EEquipmentSlots, TSubclassOf<AEquipableItem>>& ItemPair : ItemsLoadout)
	{
		if (!IsValid(ItemPair.Value))
		{
			continue;
		}

		AddEquipmentItemToSlot(ItemPair.Value, (int32)ItemPair.Key);
	}
}

void UCharacterEquipmentComponent::AutoEquip()
{
	if (AutoEquipItemInSlot != EEquipmentSlots::None)
	{
		EquipItemInSlot(AutoEquipItemInSlot);
	}
}

void UCharacterEquipmentComponent::EquipAnimationFinished()
{
	bIsEquipping = false;
	AttachCurrentItemToEquippedSocket();
}

uint32 UCharacterEquipmentComponent::NextItemsArraySlotIndex(uint32 CurrentSlotIndex)
{
	if (CurrentSlotIndex == ItemsArray.Num() - 1)
	{
		return 0;
	}
	else
	{
		return CurrentSlotIndex + 1;
	}
}

uint32 UCharacterEquipmentComponent::PreviousItemsArraySlotIndex(uint32 CurrentSlotIndex)
{
	if (CurrentSlotIndex == 0)
	{
		return ItemsArray.Num() - 1;
	}
	else
	{
		return CurrentSlotIndex - 1;
	}
}

int32 UCharacterEquipmentComponent::GetAvailableAmunitionForCurrentWeapon()
{
	check(GetCurrentRangeWeapon())
	return AmunitionArray[(uint32)GetCurrentRangeWeapon()->GetAmmoType()];
}

void UCharacterEquipmentComponent::OnWeaponReloadComplete()
{
	ReloadAmmoInCurrentWeapon();
}

void UCharacterEquipmentComponent::ReloadAmmoInCurrentWeapon(int32 NumberOfAmmo /*= 0*/, bool bCheckIsFull /*= false*/)
{
	int32 AvailableAmunition = GetAvailableAmunitionForCurrentWeapon();
	int32 CurrentAmmo = CurrentEquipedWeapon->GetAmmo();
	int32 AmmoToReload = CurrentEquipedWeapon->GetMaxAmmo() - CurrentAmmo;
	int32 ReloadedAmmo = FMath::Min(AvailableAmunition, AmmoToReload);
	if (NumberOfAmmo > 0)
	{
		ReloadedAmmo = FMath::Min(ReloadedAmmo, NumberOfAmmo);
	}

	AmunitionArray[(uint32)CurrentEquipedWeapon->GetAmmoType()] -= ReloadedAmmo;
	CurrentEquipedWeapon->SetAmmo(ReloadedAmmo + CurrentAmmo);

	if (bCheckIsFull)
	{
		AvailableAmunition = AmunitionArray[(uint32)CurrentEquipedWeapon->GetAmmoType()];
		bool bIsFullyReloaded = CurrentEquipedWeapon->GetAmmo() == CurrentEquipedWeapon->GetMaxAmmo();
		if (AvailableAmunition == 0 || bIsFullyReloaded)
		{
			CurrentEquipedWeapon->EndReload(true);
		}
	}
}

void UCharacterEquipmentComponent::OnCurrentWeaponAmmoChanged(int32 Ammo)
{
	if (OnCurrentWeaponAmmoChangedEvent.IsBound())
	{
		OnCurrentWeaponAmmoChangedEvent.Broadcast(Ammo, GetAvailableAmunitionForCurrentWeapon());
	}
}

void UCharacterEquipmentComponent::OnRep_CurrentEquippedSlot(EEquipmentSlots CurrentEquippedSlot_Old)
{
	EquipItemInSlot(CurrentEquippedSlot);
}
