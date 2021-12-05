#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCodeTypes.h"
#include "CharacterEquipmentComponent.generated.h"

typedef TArray<class AEquipableItem*, TInlineAllocator<(uint32)EEquipmentSlots::MAX>> TItemsArray;
typedef TArray<int32, TInlineAllocator<(uint32)EAmunitionType::MAX>> TAmunitionArray;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCurrentWeaponAmmoChanged, int32, int32)

class ARangeWeaponItem;
class AEquipableItem;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMECODE_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	EEquipableItemType GetCurrentEquipedItemType() const;

	ARangeWeaponItem* GetCurrentRangeWeapon() const;

	bool IsEquipping() const { return bIsEquipping; };

	void ReloadCurrentWeapon();

	FOnCurrentWeaponAmmoChanged OnCurrentWeaponAmmoChangedEvent;

	void EquipItemInSlot(EEquipmentSlots Slot);

	void AttachCurrentItemToEquippedSocket();

	void UnEquipCurrentItem();

	void EquipNextItem();
	void EquipPreviousItem();

protected:
	virtual void BeginPlay();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TMap<EAmunitionType, int32> MaxAmunitionAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TMap<EEquipmentSlots, TSubclassOf<AEquipableItem>> ItemsLoadout;
private:
	TAmunitionArray AmunitionArray;
	TItemsArray ItemsArray;

	void CreateLoadout();

	void EquipAnimationFinished();

	uint32 NextItemsArraySlotIndex(uint32 CurrentSlotIndex);
	uint32 PreviousItemsArraySlotIndex(uint32 CurrentSlotIndex);

	int32 GetAvailableAmunitionForCurrentWeapon();

	bool bIsEquipping = false;

	UFUNCTION()
	void OnWeaponReloadComplete();
	UFUNCTION()
	void OnCurrentWeaponAmmoChanged(int32 Ammo);

	FDelegateHandle OnCurrentWeaponAmmoChangedHandle;
	FDelegateHandle OnCurrentWeaponReloadedHandle;

	EEquipmentSlots CurrentEquippedSlot;
	AEquipableItem* CurrentEquippedItem;
	ARangeWeaponItem* CurrentEquipedWeapon;

	TWeakObjectPtr<class AGCBaseCharacter> CachedBaseCharacter;

	FTimerHandle EquipTimer;
};
