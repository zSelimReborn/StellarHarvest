// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Ability.h"
#include "UObject/NoExportTypes.h"
#include "EffectAbility.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class STELLARHARVEST_API UEffectAbility : public UObject, public IAbility
{
	GENERATED_BODY()

// Ability interface
public:
	virtual void Apply_Implementation(AActor* Instigator) override;

	virtual void Remove_Implementation(AActor* Instigator) override;

	virtual FName GetAbilityId_Implementation() const override;

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetDuration() const { return Duration; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCooldownDuration() const { return CooldownDuration; }
	
// Properties
protected:
	UPROPERTY(EditAnywhere, Category="Ability")
	float Duration  = 5.f;

	UPROPERTY(EditAnywhere, Category="Ability")
	float CooldownDuration = 5.f;

	UPROPERTY(EditAnywhere, Category="Ability")
	FName AbilityId;
};
