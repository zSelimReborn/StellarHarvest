// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Abilities/EffectAbility.h"

void UEffectAbility::Apply_Implementation(AActor* Instigator)
{
}

void UEffectAbility::Remove_Implementation(AActor* Instigator)
{
}

FName UEffectAbility::GetAbilityId_Implementation() const
{
	return AbilityId;
}
