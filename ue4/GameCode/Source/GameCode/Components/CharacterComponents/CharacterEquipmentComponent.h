#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCodeTypes.h"
#include "CharacterEquipmentComponent.generated.h"

typedef TArray<int32, TInlineAllocator<(uint32)EAmunitionType::MAX>> TAmunitionArray;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCurrentWeaponAmmoChanged, int32, int32)

class ARangeWeaponItem;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMECODE_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	EEquipableItemType GetCurrentEquipedItemType() const;

	ARangeWeaponItem* GetCurrentRangeWeapon() const;

	void ReloadCurrentWeapon();

	FOnCurrentWeaponAmmoChanged OnCurrentWeaponAmmoChangedEvent;

protected:
	virtual void BeginPlay();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TSubclassOf<ARangeWeaponItem> SideArmClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TMap<EAmunitionType, int32> MaxAmunitionAmount;

private:
	TAmunitionArray AmunitionArray;

	void CreateLoadout();

	int32 GetAvailableAmunitionForCurrentWeapon();
	UFUNCTION()
	void OnWeaponReloadComplete();
	UFUNCTION()
	void OnCurrentWeaponAmmoChanged(int32 Ammo);

	ARangeWeaponItem* CurrentEquipedWeapon;

	TWeakObjectPtr<class AGCBaseCharacter> CachedBaseCharacter;
};
