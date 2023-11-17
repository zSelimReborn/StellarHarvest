// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StellarHarvestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class STELLARHARVEST_API AStellarHarvestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetCrystalsGoal() const { return CrystalsGoal; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool ShouldUseTimer() const { return bUseTimer; }
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Harvest")
	int32 CrystalsGoal = 20;

	UPROPERTY(EditDefaultsOnly, Category="Harvest")
	bool bUseTimer = true;
};
