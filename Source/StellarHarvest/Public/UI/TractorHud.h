// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gameplay/Abilities/EffectAbility.h"
#include "TractorHud.generated.h"

class UEffectAbility;

/**
 * 
 */
UCLASS()
class STELLARHARVEST_API UTractorHud : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void InitializeWidget(const APlayerController* OwnerController);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCollectCrystals(const int32 CollectedCrystals, const int32 TotalCrystals);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUseCounterMeasure(const int32 CurrentAmount);

	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemyInRange();

	UFUNCTION(BlueprintImplementableEvent)
	void OnLoseEnemiesInRange();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCounterMeasureCooldownFinished();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPrimaryAbilityApplied(const UEffectAbility* Ability);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPrimaryAbilityRemoved(const UEffectAbility* Ability);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPrimaryAbilityAvailable(const UEffectAbility* Ability);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSecondaryAbilityApplied(const UEffectAbility* Ability);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSecondaryAbilityRemoved(const UEffectAbility* Ability);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnSecondaryAbilityAvailable(const UEffectAbility* Ability);

	UFUNCTION(BlueprintImplementableEvent)
	void OnNewScoreGoal();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnReachScoreGoal();

	UFUNCTION(BlueprintImplementableEvent)
	void OnGameOver(const bool bIsWinner);
};
