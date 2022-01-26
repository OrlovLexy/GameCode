#include "CharacterAttributeComponent.h"
#include "../../Characters/GCBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../../Subsystems/DebugSubsystem.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "../../GameCodeTypes.h"
#include "Net/UnrealNetwork.h"

UCharacterAttributeComponent::UCharacterAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


void UCharacterAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterAttributeComponent, Health);
}

void UCharacterAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(MaxHealth > 0.0f, TEXT("UCharacterAttributeComponent::BeginPlay() max health cannot be equal to 0"))
	checkf(GetOwner()->IsA<AGCBaseCharacter>(), TEXT("UCharacterAttributeComponent::BeginPlay can work only with AGCBaseCharacter"))
	CachedBaseCharacterOwner = StaticCast<AGCBaseCharacter*>(GetOwner());

	Health = MaxHealth;

	if (GetOwner()->HasAuthority())
	{
		CachedBaseCharacterOwner->OnTakeAnyDamage.AddDynamic(this, &UCharacterAttributeComponent::OnTakeAnyDamage);
	}
}

void UCharacterAttributeComponent::OnRep_Health()
{
	OnHealthChanged();
}

void UCharacterAttributeComponent::OnHealthChanged()
{
	if (Health <= 0.0f)
	{
		if (OnDeathEvent.IsBound())
		{
			OnDeathEvent.Broadcast();
		}
	}
}

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
void UCharacterAttributeComponent::DebugDrawAttributes()
{
	UDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
	if (!DebugSubsystem->IsCategoryEnabled(DebugCategoryCharacterAttributes))
	{
		return;
	}
	
	FVector TextLocation = CachedBaseCharacterOwner->GetActorLocation() + ((CachedBaseCharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 5.0f) * FVector::UpVector);
	DrawDebugString(GetWorld(), TextLocation, FString::Printf(TEXT("Health: %.02f"), Health), nullptr, FColor::Green, 0.0f, true);
}
#endif

void UCharacterAttributeComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (!IsAlive())
	{
		return;
	}
	
	UE_LOG(LogDamage, Warning, TEXT("UCharacterAttributeComponent::OnTakeAnyDamage %s recieved %.2f amount of damage from %s"), *CachedBaseCharacterOwner->GetName(), Damage, *DamageCauser->GetName());
	Health = FMath::Clamp (Health - Damage, 0.0f, MaxHealth);
	OnHealthChanged();
}

void UCharacterAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	DebugDrawAttributes();
#endif
}

float UCharacterAttributeComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}
