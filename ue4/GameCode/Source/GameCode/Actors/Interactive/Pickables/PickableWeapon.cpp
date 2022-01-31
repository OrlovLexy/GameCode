#include "PickableWeapon.h"
#include "GameCodeTypes.h"
#include "Utils/GCDataTableUtils.h"
#include "Inventory/Items/InventoryItem.h"
#include "Characters/GCBaseCharacter.h"

APickableWeapon::APickableWeapon()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
}

void APickableWeapon::Interact(AGCBaseCharacter* Character)
{
	FWeaponTableRow* WeaponRow = GCDataTableUtils::FindWeaponData(DataTableID);
	if (WeaponRow)
	{
		Character->AddEquipmentItem(WeaponRow->EquipableActor);
		Destroy();
	}
}

FName APickableWeapon::GetActionEventName() const
{
	return ActionInteract;
}
