#include "AIPatrollingComponent.h"
#include "Actors/Navigation/PatrollingPath.h"

bool UAIPatrollingComponent::CanPatrol() const
{
	return IsValid(PatrollingPath) && PatrollingPath->GetWaypoints().Num() > 0;
}

FVector UAIPatrollingComponent::SelectClosestWaypoint()
{
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	const TArray<FVector> Waypoints = PatrollingPath->GetWaypoints();
	FTransform PathTransform = PatrollingPath->GetActorTransform();

	FVector ClosestWaypoint;
	float MinSqDistance = FLT_MAX;
	for (int32 i = 0; i < Waypoints.Num(); ++i)
	{
		FVector Waypoint = PathTransform.TransformPosition(Waypoints[i]);
		float CurrentSqDistance = (OwnerLocation - Waypoint).SizeSquared();
		if (CurrentSqDistance < MinSqDistance)
		{
			MinSqDistance = CurrentSqDistance;
			ClosestWaypoint = Waypoint;
			CurrentWaypointIndex = i;
		}
	}

	return ClosestWaypoint;
}

FVector UAIPatrollingComponent::SelectNextWaypoint()
{
	const TArray<FVector> Waypoints = PatrollingPath->GetWaypoints();

	++CurrentWaypointIndex;
	if (CurrentWaypointIndex == Waypoints.Num())
	{
		CurrentWaypointIndex = 0;
	}

	FTransform PathTransform = PatrollingPath->GetActorTransform();
	FVector Waypoint = PathTransform.TransformPosition(Waypoints[CurrentWaypointIndex]);

	return Waypoint;
}
