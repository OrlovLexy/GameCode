// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "BasePlatform.generated.h"

UENUM(BlueprintType)
enum class EPlatformBehavior : uint8
{
	OnDemand = 0,
	Loop
};

UCLASS()
class GAMECODE_API ABasePlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePlatform();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to move platform in another position
	void MovePlatform();

	// Called from blueprint when overlap platform
	UFUNCTION(BlueprintCallable)
	void OnPlatformOverlap();

	void OnPlatformInvoked();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	FVector StartLocation;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (MakeEditWidget))
	FVector EndLocation;

	FTimeline PlatformTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* TimelineCurve;

	// Waiting time in end point
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float CooldownTime = 0.0f;

	float MovingDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPlatformBehavior PlatformBehavior = EPlatformBehavior::OnDemand;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	class APlatformInvocator* PlatformInvocator;

private:
	FTimerHandle MovingTimeHandle;
	//FTimerHandle OnDemandTimeHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlatformTimelineUpdate(float Alpha);

};
