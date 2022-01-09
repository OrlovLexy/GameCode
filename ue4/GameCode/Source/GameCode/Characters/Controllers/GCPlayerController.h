#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GCPlayerController.generated.h"

UCLASS()
class GAMECODE_API AGCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;

	bool GetIgnoreCameraPitch() const;
	void SetIgnoreCameraPitch(bool bIgnoreCameraPitch_In);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widgets")
	TSubclassOf<class UPlayerHUDWidget> PlayerHUDWidgetClass;

	virtual void SetupInputComponent() override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void TurnAtRate(float Value);
	void LookUpAtRate(float Value);
	void ChangeCrouchState();
	void ChangeProneState();
	void Mantle();
	void Jump();

	void StartSprint();
	void StopSprint();

	void SwimForward(float Value);
	void SwimRight(float Value);
	void SwimUp(float Value);

	void ClimbLadderUp(float Value);
	void InteractWithLadder();

	void PlayerStartFire();
	void PlayerStopFire();

	void StartAiming();
	void StopAiming();

	void NextItem();
	void PreviousItem();
	void EquipPrimaryItem();

	void Reload();

	void PrimaryMeleeAttack();
	void SecondaryMeleeAttack();

	TSoftObjectPtr<class AGCBaseCharacter> CachedBaseCharacter;
	
	bool bIgnoreCameraPitch = false;

	void CreateAndInitializeWidgets();

	UPlayerHUDWidget* PlayerHUDWidget = nullptr;
};
