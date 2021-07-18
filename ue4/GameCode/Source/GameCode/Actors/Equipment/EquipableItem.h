#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCodeTypes.h"
#include "EquipableItem.generated.h"

UCLASS(Abstract, NotBlueprintable)
class GAMECODE_API AEquipableItem : public AActor
{
	GENERATED_BODY()
public:
	EEquipableItemType GetItemType() const { return ItemType; };

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipable item")
	EEquipableItemType ItemType = EEquipableItemType::None;
};
