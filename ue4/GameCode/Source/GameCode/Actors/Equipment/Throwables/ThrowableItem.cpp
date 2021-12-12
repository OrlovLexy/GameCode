#include "ThrowableItem.h"
#include "Characters/GCBaseCharacter.h"
#include "Actors/Projectiles/GCProjectile.h"

void AThrowableItem::Throw()
{
	checkf(GetOwner() - IsA<AGCBaseCharacter>(), TEXT("AThrowableItem::Throw only character can be an owner of range weapon"))
	AGCBaseCharacter* CharacterOwner = StaticCast<AGCBaseCharacter*>(GetOwner());

	APlayerController* Controller = CharacterOwner->GetController<APlayerController>();
	if (!IsValid(Controller))
	{
		return;
	}

	FVector PlayerViewPoint;
	FRotator PlayerViewRotation;

	Controller->GetPlayerViewPoint(PlayerViewPoint, PlayerViewRotation);

	FVector ViewDirection = PlayerViewRotation.RotateVector(FVector::ForwardVector);

	FVector SpawnLocation = PlayerViewPoint + ViewDirection * 100.0f;

	AGCProjectile* Projectile = GetWorld()->SpawnActor<AGCProjectile>(ProjectileClass, SpawnLocation, FRotator::ZeroRotator);
	if (IsValid(Projectile))
	{
		Projectile->SetOwner(GetOwner());
		Projectile->LaunchProjectile(ViewDirection);
	}
}
