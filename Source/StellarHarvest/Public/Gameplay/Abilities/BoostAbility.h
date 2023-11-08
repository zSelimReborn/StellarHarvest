// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/EffectAbility.h"
#include "BoostAbility.generated.h"

/**
 * 
 */
UCLASS(meta=(ToolTip="Works only with actor having UCharacterMovementComponent"))
class STELLARHARVEST_API UBoostAbility : public UEffectAbility
{
	GENERATED_BODY()

public:
	virtual void Apply_Implementation(AActor* Instigator) override;

	virtual void Remove_Implementation(AActor* Instigator) override;
	
// Properties
protected:
	UPROPERTY(EditAnywhere, Category="Ability|Boost")
	float BoostDelta = 500.f;
};
