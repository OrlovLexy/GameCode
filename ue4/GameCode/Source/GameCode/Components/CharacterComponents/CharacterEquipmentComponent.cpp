#include "CharacterEquipmentComponent.h"
#include "Characters/GCBaseCharacter.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "Actors/Equipment/Throwables/ThrowableItem.h"
#include "Actors/Equipment/Weapons/MeleeWeaponItem.h"

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
		CurrentEquippedSlot = Slot;
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

void UCharacterEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->IsA<AGCBaseCharacter>(), TEXT("UCharacterEquipmentComponent can be used only with AGCBaseCharacter"))
	CachedBaseCharacter = StaticCast<AGCBaseCharacter*>(GetOwner());

	CreateLoadout();
}

void UCharacterEquipmentComponent::CreateLoadout()
{
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
		AEquipableItem* Item = GetWorld()->SpawnActor<AEquipableItem>(ItemPair.Value);
		Item->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Item->GetUnEquippedSocketName());
		Item->SetOwner(CachedBaseCharacter.Get());
		Item->UnEquip();
		ItemsArray[(uint32)ItemPair.Key] = Item;
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
