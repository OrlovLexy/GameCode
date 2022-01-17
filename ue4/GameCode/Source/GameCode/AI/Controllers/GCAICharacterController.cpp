#include "GCAICharacterController.h"
#include "AI/Characters/GCAICharacter.h"
#include "Perception/AISense_Sight.h"
#include "Components/CharacterComponents/AIPatrollingComponent.h"

void AGCAICharacterController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (IsValid(InPawn))
	{
		checkf(InPawn->IsA<AGCAICharacter>(), TEXT("AGCAICharacterController::SetPawn GCAICharacterController can possess only GCAICharacter"));
		CachedAICharacter = StaticCast<AGCAICharacter*>(InPawn);
	}
	else
	{
		CachedAICharacter = nullptr;
	}
}

void AGCAICharacterController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);

	if (!CachedAICharacter.IsValid())
	{
		return;
	}

	TryMoveToNextTarget();
}

void AGCAICharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	if (!Result.IsSuccess())
	{
		return;
	}
	TryMoveToNextTarget();
}

void AGCAICharacterController::BeginPlay()
{
	Super::BeginPlay();
	UAIPatrollingComponent* PatrollingComponent = CachedAICharacter->GetAIPatrollingComponent();
	if (PatrollingComponent->CanPatrol())
	{
		FVector ClosestWaypoint = PatrollingComponent->SelectClosestWaypoint();
		MoveToLocation(ClosestWaypoint);
		bIsPatrolling = true;
	}
}

void AGCAICharacterController::TryMoveToNextTarget()
{
	UAIPatrollingComponent* PatrollingComponent = CachedAICharacter->GetAIPatrollingComponent();

	AActor* ClosestActor = GetClosestSensedActors(UAISense_Sight::StaticClass());
	if (IsValid(ClosestActor))
	{
		if (!IsTargetReached(ClosestActor->GetActorLocation()))
		{
			MoveToActor(ClosestActor);
		}
		bIsPatrolling = false;
	}
	else if (PatrollingComponent->CanPatrol())
	{
		FVector Waypoint = bIsPatrolling ? PatrollingComponent->SelectNextWaypoint() : PatrollingComponent->SelectClosestWaypoint();
		if (!IsTargetReached(Waypoint))
		{
			MoveToLocation(Waypoint);
		}
		bIsPatrolling = true;
	}
}

bool AGCAICharacterController::IsTargetReached(FVector TargetLocation) const
{
	return (TargetLocation - CachedAICharacter->GetActorLocation()).SizeSquared() < FMath::Square(TargetReachRadius);
}
