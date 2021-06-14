// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlatform.h"
#include "PlatformInvocator.h"

// Sets default values
ABasePlatform::ABasePlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* DefaultPlatformRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Platform root"));
	RootComponent = DefaultPlatformRoot;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	PlatformMesh->SetupAttachment(DefaultPlatformRoot);
}


// Called when the game starts or when spawned
void ABasePlatform::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = PlatformMesh->GetRelativeLocation();

	if (IsValid(TimelineCurve))
	{
		FOnTimelineFloatStatic PlatformMovementTimelineUpdate;
		PlatformMovementTimelineUpdate.BindUObject(this, &ABasePlatform::PlatformTimelineUpdate);
		PlatformTimeline.AddInterpFloat(TimelineCurve, PlatformMovementTimelineUpdate);
	}

	// Subscribe to delegate
	if (IsValid(PlatformInvocator))
	{
		PlatformInvocator->OnInvocatorActivated.AddUObject(this, &ABasePlatform::OnPlatformInvoked);
	}
	
	// Calculate time needed for reverse or loop moving
	MovingDelay = PlatformTimeline.GetTimelineLength() + CooldownTime;

	// Set timer for loop platform
	if (PlatformBehavior == EPlatformBehavior::Loop)
	{
		MovePlatform();
		GetWorldTimerManager().SetTimer(MovingTimeHandle, this, &ABasePlatform::MovePlatform, MovingDelay, true);
	}
}

// Called to move platform in another position
void ABasePlatform::MovePlatform()
{
		FVector CurrentLocation = PlatformMesh->GetRelativeLocation();
		if (CurrentLocation == StartLocation) { PlatformTimeline.Play(); }
		if (CurrentLocation == EndLocation) { PlatformTimeline.Reverse(); }
}

// Called from blueprint when overlap platform
void ABasePlatform::OnPlatformOverlap()
{
	// Moving platform if not waiting overlap from Platform Invocator
	if (!IsValid(PlatformInvocator))
	{
		MovePlatform();
	}
}

// Called when Platform Invocator overlapped
void ABasePlatform::OnPlatformInvoked()
{
	if (!GetWorldTimerManager().IsTimerActive(MovingTimeHandle))
	{
		MovePlatform();
		GetWorldTimerManager().SetTimer(MovingTimeHandle, this, &ABasePlatform::MovePlatform, MovingDelay, false);
	}
}

// Called every frame
void ABasePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlatformTimeline.TickTimeline(DeltaTime);
}

void ABasePlatform::PlatformTimelineUpdate(const float Alpha)
{
	const FVector PlatformTargetLocation = FMath::Lerp(StartLocation, EndLocation, Alpha);
	PlatformMesh->SetRelativeLocation(PlatformTargetLocation);
}
