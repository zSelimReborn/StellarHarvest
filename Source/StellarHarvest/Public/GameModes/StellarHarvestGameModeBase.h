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
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Harvest")
	int32 CrystalsGoal = 20;
};
