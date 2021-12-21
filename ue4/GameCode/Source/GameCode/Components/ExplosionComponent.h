#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ExplosionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExplosion);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMECODE_API UExplosionComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable)
	void Explode(AController* Controller);

	UPROPERTY(BlueprintAssignable)
	FOnExplosion OnExplosion;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Damage", meta = (ClampMin = 1.0f, UIMin = 1.0f))
	float MaxDamage = 75.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Damage", meta = (ClampMin = 1.0f, UIMin = 1.0f))
	float MinDamage = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Damage", meta = (ClampMin = 1.0f, UIMin = 1.0f))
	float DamageFalloff = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Damage")
	TSubclassOf<class UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Radius", meta = (ClampMin = 1.0f, UIMin = 1.0f))
	float InnerRadius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Radius", meta = (ClampMin = 1.0f, UIMin = 1.0f))
	float OuterRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | VFX")
	UParticleSystem* ExplosionVFX;		
};
