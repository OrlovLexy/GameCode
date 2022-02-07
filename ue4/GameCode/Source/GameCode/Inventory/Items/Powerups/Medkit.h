#pragma once

#include "CoreMinimal.h"
#include "Inventory/Items/InventoryItem.h"
#include "Medkit.generated.h"

UCLASS()
class GAMECODE_API UMedkit : public UInventoryItem
{
	GENERATED_BODY()

public:
	virtual bool Consume(AGCBaseCharacter* ConsumeTarget) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Medkit")
	float Health = 25.0f;
};
