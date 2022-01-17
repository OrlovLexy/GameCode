#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GCAIController.generated.h"

class UAISense;
UCLASS()
class GAMECODE_API AGCAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AGCAIController();

protected:
	AActor* GetClosestSensedActors(TSubclassOf<UAISense> SenseClass) const;
};
