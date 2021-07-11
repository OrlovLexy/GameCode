#include "CharacterEquipmentComponent.h"
#include "Characters/GCBaseCharacter.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "GameCodeTypes.h"

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
	CurrentEquipedItem = GetWorld()->SpawnActor<ARangeWeaponItem>(SideArmClass);
	
	CurrentEquipedItem->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketCharacterWeapon);
}
