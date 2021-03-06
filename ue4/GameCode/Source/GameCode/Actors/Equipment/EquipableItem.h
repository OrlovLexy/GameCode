#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCodeTypes.h"
#include "EquipableItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentStateChanged, bool, bIsEquipped);

class UAnimMontage;
class AGCBaseCharacter;
UCLASS(Abstract, NotBlueprintable)
class GAMECODE_API AEquipableItem : public AActor
{
	GENERATED_BODY()
public:
	AEquipableItem();

	virtual void SetOwner(AActor* NewOwner) override;

	EEquipableItemType GetItemType() const { return ItemType; };

	UAnimMontage* GetCharacterEquipAnimMontage() const { return CharacterEquipAnimMontage; };

	FName GetUnEquippedSocketName() const { return UnEquippedSocketName; };
	FName GetEquippedSocketName() const { return EquippedSocketName; };

	virtual void Equip();
	virtual void UnEquip();

	virtual EReticleType GetReticleType() const;

	FName GetDataTableID() const;

	bool IsSlotCompatible(EEquipmentSlots Slot);

protected:
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentStateChanged OnEquipmentStateChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipable item")
	EEquipableItemType ItemType = EEquipableItemType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipable item")
	UAnimMontage* CharacterEquipAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipable item")
	FName UnEquippedSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipable item")
	FName EquippedSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipable item")
	TArray<EEquipmentSlots> CompatibleEquipmentSlots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reticle")
	EReticleType ReticleType = EReticleType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory item")
	FName DataTableID = NAME_None;

	AGCBaseCharacter* GetCharacterOwner() const;

private:
	TWeakObjectPtr<AGCBaseCharacter> CachedCharacterOwner;
};
