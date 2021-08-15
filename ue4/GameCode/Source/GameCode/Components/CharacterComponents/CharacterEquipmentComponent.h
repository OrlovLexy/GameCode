#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCodeTypes.h"
#include "CharacterEquipmentComponent.generated.h"

class ARangeWeaponItem;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMECODE_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	EEquipableItemType GetCurrentEquipedItemType() const;

	ARangeWeaponItem* GetCurrentRangeWeapon() const;

	void Fire();

protected:
	virtual void BeginPlay();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TSubclassOf<ARangeWeaponItem> SideArmClass;

private:
	void CreateLoadout();

	ARangeWeaponItem* CurrentEquipedWeapon;

	TWeakObjectPtr<class AGCBaseCharacter> CachedBaseCharacter;
};
