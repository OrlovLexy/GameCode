#pragma once

#include "CoreMinimal.h"
#include "Characters/GCBaseCharacter.h"
#include "GCAICharacter.generated.h"

class UBehaviorTree;
class UAIPatrollingComponent;
UCLASS(Blueprintable)
class GAMECODE_API AGCAICharacter : public AGCBaseCharacter
{
	GENERATED_BODY()
	
public:
	AGCAICharacter(const FObjectInitializer& ObjectInitializer);

	UAIPatrollingComponent* GetAIPatrollingComponent() const;

	UBehaviorTree* GetBehaviorTree() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPatrollingComponent* AIPatrollingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
};
