#include "GCAICharacter.h"
#include "Components/CharacterComponents/AIPatrollingComponent.h"

AGCAICharacter::AGCAICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIPatrollingComponent = CreateDefaultSubobject<UAIPatrollingComponent>(TEXT("AIPatrolling"));
}

UAIPatrollingComponent* AGCAICharacter::GetAIPatrollingComponent() const
{
	return AIPatrollingComponent;
}
