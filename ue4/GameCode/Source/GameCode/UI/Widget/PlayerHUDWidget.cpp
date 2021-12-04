// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "Characters/GCBaseCharacter.h"
#include "Components/CharacterComponents/CharacterAttributeComponent.h"
#include "Blueprint/WidgetTree.h"
#include "ReticleWidget.h"
#include "AmmoWidget.h"

UReticleWidget* UPlayerHUDWidget::GetReticleWidget()
{
	return WidgetTree->FindWidget<UReticleWidget>(ReticleWidgetName);
}

class UAmmoWidget* UPlayerHUDWidget::GetAmmoWidget()
{
	return WidgetTree->FindWidget<UAmmoWidget>(AmmoWidgetName);
}

float UPlayerHUDWidget::GetHealthPercent() const
{
	float Result = 1.0f;
	APawn* Pawn = GetOwningPlayerPawn();
	AGCBaseCharacter* Character = Cast<AGCBaseCharacter>(Pawn);
	if (IsValid(Character))
	{
		const UCharacterAttributeComponent* CharacterAttribute = Character->GetCharacterAttributeComponent();
		Result = CharacterAttribute->GetHealthPercent();
	}
	return Result;
}
