// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeaponItem.h"
#include "Components/Weapon/WeaponBarellComponent.h"
#include "GameCodeTypes.h"
#include "Characters/GCBaseCharacter.h"

ARangeWeaponItem::ARangeWeaponItem()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponBarell = CreateDefaultSubobject<UWeaponBarellComponent>(TEXT("WeaponBarell"));
	WeaponBarell->SetupAttachment(WeaponMesh, SocketWeaponMuzzle);
}

void ARangeWeaponItem::Fire()
{
	checkf(GetOwner()-IsA<AGCBaseCharacter>(), TEXT("ARangeWeaponItem::Fire() only character can be an owner of range weapon"))
	AGCBaseCharacter* CharacterOwner = StaticCast<AGCBaseCharacter*>(GetOwner());

	CharacterOwner->PlayAnimMontage(CharacterFireMontage);
	PlayAnimMontage(WeaponFireMontage);

	APlayerController* Controller = CharacterOwner->GetController<APlayerController>();
	
	if (!IsValid(Controller))
	{
		return;
	}

	FVector PlayerViewPoint;
	FRotator PlayerViewRotation;

	Controller->GetPlayerViewPoint(PlayerViewPoint, PlayerViewRotation);

	FVector ViewDirection = PlayerViewRotation.RotateVector(FVector::ForwardVector);

	WeaponBarell->Shot(PlayerViewPoint, ViewDirection, Controller);
}

float ARangeWeaponItem::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	return WeaponAnimInstance->Montage_Play(AnimMontage);
}
