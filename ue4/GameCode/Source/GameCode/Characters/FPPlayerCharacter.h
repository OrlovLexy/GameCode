#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "FPPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GAMECODE_API AFPPlayerCharacter : public APlayerCharacter
{
	GENERATED_BODY()
	
public:
	AFPPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual FRotator GetViewRotation() const override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PrevCustomMode = 0) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | First Person")
	class USkeletalMeshComponent* FirstPersonMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | First Person")
	class UCameraComponent* FirstPersonCameraComponent;

	virtual void OnMantle(const FMantlingSettings& MantlingSettings, float MantlingAnimationStartTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First Person | Camera | Ladder | Pitch", meta = (UIMin = -89.0f, UIMax = 89.0f))
	float LadderCameraMinPitch = -60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First Person | Camera | Ladder | Pitch", meta = (UIMin = -89.0f, UIMax = 89.0f))
	float LadderCameraMaxPitch = 80.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First Person | Camera | Ladder | Yaw", meta = (UIMin = 0.0f, UIMax = 359.0f))
	float LadderCameraMinYaw = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First Person | Camera | Ladder | Yaw", meta = (UIMin = 0.0f, UIMax = 359.0f))
	float LadderCameraMaxYaw = 175.0f;

private:
	FTimerHandle FPMontageTimer;

	void OnFPMontageTimerElapsed();
	bool IsFPMontagePlaying() const;

	TWeakObjectPtr<class AGCPlayerController> GCPlayerController;

	void OnLadderCameraViewChanged(bool bIsOnLadder);
};
