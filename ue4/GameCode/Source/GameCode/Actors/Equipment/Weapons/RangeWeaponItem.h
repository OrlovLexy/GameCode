// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipableItem.h"
#include "RangeWeaponItem.generated.h"

UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	Single,
	FullAuto
};

class UAnimMontage;
UCLASS(Blueprintable)
class GAMECODE_API ARangeWeaponItem : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	ARangeWeaponItem();

	void StartFire();
	void StopFire();

	FTransform GetForeGripTransform() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWeaponBarellComponent* WeaponBarell;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Weapon")
	UAnimMontage* WeaponFireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Character")
	UAnimMontage* CharacterFireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters")
	EWeaponFireMode WeaponFireMode = EWeaponFireMode::Single;

	// Rate of fire in rounds per minute
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters", meta = (ClampMin = 1.0f, UIMin = 1.0f))
	float RateOfFire = 600.0f;

private:
	void MakeShot();

	float GetShotTimerInterval();

	float PlayAnimMontage(UAnimMontage* AnimMontage);

	FTimerHandle ShotTimer;
};