// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Abilities/CloakAbility.h"

#include "Characters/StellarBaseCharacter.h"

void UCloakAbility::Apply_Implementation(AActor* Instigator)
{
	AStellarBaseCharacter* Character = Cast<AStellarBaseCharacter>(Instigator);
	if (Character == nullptr)
	{
		return;
	}

	Character->SetInvisible();
}

void UCloakAbility::Remove_Implementation(AActor* Instigator)
{
	AStellarBaseCharacter* Character = Cast<AStellarBaseCharacter>(Instigator);
	if (Character == nullptr)
	{
		return;
	}

	Character->SetVisible();
}
