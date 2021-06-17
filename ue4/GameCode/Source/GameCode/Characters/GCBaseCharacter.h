// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GCBaseCharacter.generated.h"

USTRUCT(BlueprintType)
struct FMantlingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* MantlingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveVector* MantlingCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionXY = 65.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionZ = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeight = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeight = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeightStartTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeightStartTime = 0.5f;
};

class AInteractiveActor;
class UGCBaseCharacterMovementComponent;
UCLASS(Abstract, NotBlueprintable)
class GAMECODE_API AGCBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGCBaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void MoveForward(float Value) {};
	virtual void MoveRight(float Value) {};
	virtual void Turn(float Value) {};
	virtual void LookUp(float Value) {};
	virtual void TurnAtRate(float Value) {};
	virtual void LookUpAtRate(float Value) {};
	virtual void Jump() override;
	virtual void ChangeCrouchState();
	virtual void ChangeProneState();
	virtual void StartSprint();
	virtual void StopSprint();

	virtual void Tick(float DeltaTime) override;

	virtual void SwimForward(float Value) {};
	virtual void SwimRight(float Value) {};
	virtual void SwimUp(float Value) {};

	bool bCanJump = true;

	virtual void Mantle(bool bForce = false);

	bool CanMantle() const;

	virtual bool CanJumpInternal_Implementation() const override;

	UGCBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() const { return GCBaseCharacterMovementComponent; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetIKRightFootOffset() const { return IKRightFootOffset; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetIKLeftFootOffset() const { return IKLeftFootOffset; }
		
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetIKPelvisOffset() const { return IKPelvisOffset; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetPronePelvisOffset() const { return PronePelvisOffset; }

	UPROPERTY(BlueprintReadOnly, Category = "Character | Prone")
	bool bIsProned = false;

	UFUNCTION(BlueprintCallable, Category = "Character | Prone")
	virtual void Prone();

	UFUNCTION(BlueprintCallable, Category = "Character | Prone")
	virtual void UnProne(bool bIsFullHeight);

	UFUNCTION(BlueprintCallable, Category = "Character | Prone")
	virtual bool CanProne() const;

	virtual void OnStartProne(float HalfHeightAdjust, float ScaledHeightAdjust);
	virtual void OnEndProne(float HalfHeightAdjust, float ScaledHeightAdjust);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnStartProne", ScriptName = "OnStartProne"))
	void K2_OnStartProne(float HalfHeightAdjust, float ScaledHeightAdjust);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnEndProne", ScriptName = "OnEndProne"))
	void K2_OnEndProne(float HalfHeightAdjust, float ScaledHeightAdjust);

	virtual void RecalculateBaseEyeHeight() override;

	void RegisterInteractiveActor(AInteractiveActor* InteractiveActor);
	void UnregisterInteractiveActor(AInteractiveActor* InteractiveActor);

	void ClimbLadderUp(float Value);
	void InteractWithLadder();
	const class ALadder* GetAvailableLadder() const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintStart();
	virtual void OnSprintStart_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintEnd();
	virtual void OnSprintEnd_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Controls")
	float BaseTurnRate = 45.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Controls")
	float BaseLookUpRate = 45.0f;

	virtual bool CanSprint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Movement")
	class ULedgeDetectorComponent* LedgeDetectorComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling")
	FMantlingSettings HighMantleSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling")
	FMantlingSettings LowMantleSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float LowMantleMaxHeight = 125.0f;

	UGCBaseCharacterMovementComponent* GCBaseCharacterMovementComponent;

	// ~ begin IK settings
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | IK settings")
	FName RightFootSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | IK settings")
	FName LeftFootSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | IK settings", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float IKTraceDistance = 30.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Character | IK settings", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float IKInterpSpeed = 20.0f;
	// ~ end IK settings

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Movement", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxStamina = 50.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Movement", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float StaminaRestoreVelocity = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Movement", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float SprintStaminaConsumptionVelocity = 5.0f;
private:
	void TryChangeSprintState(float DeltaSeconds);
	float GetIKOffsetForASocket(const FName& SocketName);
	float CalculateIKPelvisOffset();

	const FMantlingSettings& GetMantlingSettings(float LedgeHeight) const;

	TArray<AInteractiveActor*> AvailableInteractiveActors;

	bool bIsSprintRequsted = false;

	float CurrentStamina = 0.0f;

	float PronePelvisOffset = 0.0f;
	float PronedEyeHeight = 0.0f;

	// ~ begin IK settings
	float IKRightFootOffset = 0.0f;
	float IKLeftFootOffset = 0.0f;
	float IKPelvisOffset = 0.0f;				   
	// ~ end IK settings
};
