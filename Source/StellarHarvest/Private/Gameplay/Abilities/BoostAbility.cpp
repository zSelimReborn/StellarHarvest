// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Abilities/BoostAbility.h"

#include "GameFramework/CharacterMovementComponent.h"

void UBoostAbility::Apply_Implementation(AActor* Instigator)
{
	if (Instigator == nullptr)
	{
		return;
	}
	
	UCharacterMovementComponent* MovementComponent = Instigator->FindComponentByClass<UCharacterMovementComponent>();
	if (MovementComponent == nullptr)
	{
		return;
	}
	
	MovementComponent->MaxWalkSpeed += BoostDelta;
}

void UBoostAbility::Remove_Implementation(AActor* Instigator)
{
	if (Instigator == nullptr)
	{
		return;
	}
	
	UCharacterMovementComponent* MovementComponent = Instigator->FindComponentByClass<UCharacterMovementComponent>();
	if (MovementComponent == nullptr)
	{
		return;
	}
	
	MovementComponent->MaxWalkSpeed -= BoostDelta;
}
