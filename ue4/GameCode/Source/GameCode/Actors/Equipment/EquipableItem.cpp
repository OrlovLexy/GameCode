#include "EquipableItem.h"
#include "Characters/GCBaseCharacter.h"

AEquipableItem::AEquipableItem()
{
	SetReplicates(true);
}

void AEquipableItem::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	if (IsValid(NewOwner))
	{
		checkf(GetOwner() - IsA<AGCBaseCharacter>(), TEXT("AEquipableItem::SetOwner only character can be an owner of an equipable item"))
		CachedCharacterOwner = StaticCast<AGCBaseCharacter*>(GetOwner());
		if (GetLocalRole() == ROLE_Authority)
		{
			SetAutonomousProxy(true);
		}
	}
	else
	{
		CachedCharacterOwner = nullptr;
	}
}

void AEquipableItem::Equip()
{
	if (OnEquipmentStateChanged.IsBound())
	{
		OnEquipmentStateChanged.Broadcast(true);
	}
}

void AEquipableItem::UnEquip()
{
	if (OnEquipmentStateChanged.IsBound())
	{
		OnEquipmentStateChanged.Broadcast(false);
	}
}

EReticleType AEquipableItem::GetReticleType() const
{
	return ReticleType;
}

AGCBaseCharacter* AEquipableItem::GetCharacterOwner() const
{
	return CachedCharacterOwner.IsValid() ? CachedCharacterOwner.Get() : nullptr;
}
