#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCodeTypes.h"
#include "WeaponWheelWidget.generated.h"

struct FWeaponTableRow;
class UCharacterEquipmentComponent;
class UImage;
class UTextBlock;
UCLASS()
class GAMECODE_API UWeaponWheelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeWeaponWheelWidget(UCharacterEquipmentComponent* EquipmentComponent);

	void NextSegment();
	void PreviousSegment();
	void ConfirmSelection();
	
protected:
	virtual void NativeConstruct() override;
	void SelectSegment();

	UPROPERTY(meta = (BindWidget))
	UImage* RadialBackground;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponNameText;

	//Array for correspondence between equipment slots and radial menu segments (array indices)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon wheel settings")
	TArray<EEquipmentSlots> EquipmentSlotsSegments;

private:
	FWeaponTableRow* GetTableRowForSegment(int32 SegmentIndex) const;

	int32 CurrentSegmentIndex;
	UMaterialInstanceDynamic* BackgroundMaterial;
	TWeakObjectPtr<UCharacterEquipmentComponent> LinkedEquipmentComponent;
};
