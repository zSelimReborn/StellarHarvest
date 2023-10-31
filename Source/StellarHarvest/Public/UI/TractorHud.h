// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TractorHud.generated.h"

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
};
