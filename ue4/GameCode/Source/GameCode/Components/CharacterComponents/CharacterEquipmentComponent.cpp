#include "CharacterEquipmentComponent.h"
#include "Characters/GCBaseCharacter.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"

EEquipableItemType UCharacterEquipmentComponent::GetCurrentEquipedItemType() const
{
	EEquipableItemType Result = EEquipableItemType::None;
	if (IsValid(CurrentEquipedWeapon))
	{
		Result = CurrentEquipedWeapon->GetItemType();
	}
	return Result;
}

ARangeWeaponItem* UCharacterEquipmentComponent::GetCurrentRangeWeapon() const
{
	return CurrentEquipedWeapon;
}

void UCharacterEquipmentComponent::Fire()
{
	if (IsValid(CurrentEquipedWeapon))
	{
		CurrentEquipedWeapon->Fire();
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
	if (!IsValid(SideArmClass))
	{
		return;
	}
	CurrentEquipedWeapon = GetWorld()->SpawnActor<ARangeWeaponItem>(SideArmClass);
	
	CurrentEquipedWeapon->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketCharacterWeapon);
	CurrentEquipedWeapon->SetOwner(CachedBaseCharacter.Get());
}
