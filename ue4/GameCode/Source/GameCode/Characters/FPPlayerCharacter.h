#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "FPPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GAMECODE_API AFPPlayerCharacter : public APlayerCharacter
{
	GENERATED_BODY()
	
public:
	AFPPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | First Person")
	class USkeletalMeshComponent* FirstPersonMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | First Person")
	class UCameraComponent* FirstPersonCameraComponent;
};
