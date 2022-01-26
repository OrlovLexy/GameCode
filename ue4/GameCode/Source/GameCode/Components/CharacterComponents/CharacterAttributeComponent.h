#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathEventSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMECODE_API UCharacterAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterAttributeComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnDeathEventSignature OnDeathEvent;

	bool IsAlive() { return Health > 0.0f; }

	float GetHealthPercent() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health", meta = (UIMin=0.0f))
	float MaxHealth = 100.0f;

private:
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float Health = 0.0f;

	UFUNCTION()
	void OnRep_Health();

	void OnHealthChanged();

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	void DebugDrawAttributes();
#endif

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	TWeakObjectPtr<class AGCBaseCharacter> CachedBaseCharacterOwner;
};
