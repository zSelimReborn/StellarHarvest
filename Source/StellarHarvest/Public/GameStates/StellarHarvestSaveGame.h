// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "StellarHarvestSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class STELLARHARVEST_API UStellarHarvestSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetBestTime(const float Time) { BestTime = Time; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetBestTime() const { return BestTime; }
	
protected:
	UPROPERTY()
	float BestTime;
};
