// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrystalClusterWidget.generated.h"

class ACrystal;

/**
 * 
 */
UCLASS()
class STELLARHARVEST_API UCrystalClusterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnInitializeProperties(ACrystal* CrystalCluster, const int32 CrystalsAmount, const float TimeToHarvest);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartHarvesting(AActor* HarvestingActor);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStopHarvesting(AActor* HarvestingActor);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHarvestCrystals(const int32 HarvestedCrystals, const int32 CurrentCrystalsAmount);
};
