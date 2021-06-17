// Fill out your copyright notice in the Description page of Project Settings.


#include "GCBaseCharacterAnimInstance.h"
#include "../GCBaseCharacter.h"
#include "../../Components/MovementComponents/GCBaseCharacterMovementComponent.h"

void UGCBaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<AGCBaseCharacter>(), TEXT("UGCBaseCharacterAnimInstance::NativeBeginPlay() UGCBaseCharacterAnimInstance can be used only with AGCBaseCharacter"));
	CachedBaseCharacter = StaticCast<AGCBaseCharacter*>(TryGetPawnOwner());
}

void UGCBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedBaseCharacter.IsValid())
	{
		return;
	}
	UGCBaseCharacterMovementComponent* CharacterMovement = CachedBaseCharacter->GetBaseCharacterMovementComponent();
	Speed = CharacterMovement->Velocity.Size();
	bIsFalling = CharacterMovement->IsFalling();
	bIsCrouching = CharacterMovement->IsCrouching();
	bIsSprinting = CharacterMovement->IsSprinting();
	bIsProning = CharacterMovement->IsProning();
	bIsOutOfStamina = CharacterMovement->IsOutOfStamina();
	bIsSwimming = CharacterMovement->IsSwimming();
	bIsOnLadder = CharacterMovement->IsOnLadder();
	if (bIsOnLadder)
	{
		LadderSpeedRatio = CharacterMovement->GetLadderSpeedRatio();
	}

	RightFootEffectorLocation = FVector(CachedBaseCharacter->GetIKRightFootOffset() + CachedBaseCharacter->GetIKPelvisOffset(), 0.0f, 0.0f);
	LeftFootEffectorLocation = FVector(-(CachedBaseCharacter->GetIKLeftFootOffset() + CachedBaseCharacter->GetIKPelvisOffset()), 0.0f, 0.0f);
	PelvisOffsetLocation = FVector(0.0f, 0.0f, CachedBaseCharacter->GetIKPelvisOffset());
	PronePelvisOffsetLocation = FVector(0.0f, -CachedBaseCharacter->GetPronePelvisOffset(), 0.0f);
}
