// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/EffectAbility.h"
#include "CloakAbility.generated.h"

/**
 * 
 */
UCLASS()
class STELLARHARVEST_API UCloakAbility : public UEffectAbility
{
	GENERATED_BODY()

public:
	virtual void Apply_Implementation(AActor* Instigator) override;

	virtual void Remove_Implementation(AActor* Instigator) override;
};
