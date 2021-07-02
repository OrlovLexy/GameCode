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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | First Person")
	class USkeletalMeshComponent* FirstPersonMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | First Person")
	class UCameraComponent* FirstPersonCameraComponent;
};
