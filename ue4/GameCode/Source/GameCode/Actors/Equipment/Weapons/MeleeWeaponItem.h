#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipableItem.h"
#include "GameCodeTypes.h"
#include "MeleeWeaponItem.generated.h"

USTRUCT(BlueprintType)
struct FMeleeAttackDesription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee attack")
	TSubclassOf<class UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee attack", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float DamageAmount = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee attack")
	class UAnimMontage* AttackMontage;
};

UCLASS(Blueprintable)
class GAMECODE_API AMeleeWeaponItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	AMeleeWeaponItem();

	void StartAttack(EMeleeAttackTypes AttackType);

	void SetIsHitRegistrationEnabled(bool bIsRegistrationEnabled);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee attack")
	TMap<EMeleeAttackTypes, FMeleeAttackDesription> Attacks;

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void ProcessHit(const FHitResult& HitResult, const FVector& HitDirection);

	void OnAttackTimerElapsed();

	TArray<class UMeleeHitRegistrator*> HitRegistrators;
	TSet<AActor*> HitActors;

	FMeleeAttackDesription* CurrentAttack;
	FTimerHandle AttackTimer;
};
