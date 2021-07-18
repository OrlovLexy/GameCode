#pragma once

#define ECC_Climbing ECC_GameTraceChannel1
#define ECC_InteractionVolume ECC_GameTraceChannel2

const FName SocketFPCamera = FName("CameraSocket");
const FName SocketCharacterWeapon = FName("CharacterWeaponSocket");
const FName SocketWeaponMuzzle = FName("MuzzleSocket");

const FName CollisionProfilePawn = FName("Pawn");
const FName CollisionProfilePawnInteractionVolume = FName("PawnInteractionVolume");
const FName CollisionProfileRagDoll = FName("RagDoll");

const FName DebugCategoryLedgeDetection = FName("LedgeDetection");
const FName DebugCategoryIKDetection = FName("IKDetection");
const FName DebugCategoryCharacterAttributes= FName("CharacterAttributes");

UENUM(BlueprintType)
enum class EEquipableItemType : uint8
{
	None,
	Pistol
};
