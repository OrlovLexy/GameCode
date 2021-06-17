// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GCBaseCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMECODE_API UGCBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	float Speed = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsCrouching= false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsProning = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsOutOfStamina = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsSwimming = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsOnLadder = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	float LadderSpeedRatio = 0.0f;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Character|IK settings")
	FVector RightFootEffectorLocation = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Character|IK settings")
	FVector LeftFootEffectorLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Character|IK settings")
	FVector PelvisOffsetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Character|IK settings")
	FVector PronePelvisOffsetLocation = FVector::ZeroVector;

private:
	TWeakObjectPtr<class AGCBaseCharacter> CachedBaseCharacter;
};
