// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ActorUtils.generated.h"

/**
 * 
 */
UCLASS()
class STELLARHARVEST_API UActorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static bool IsVisibleOnScreen(const APlayerController* PC, const AActor* TestingActor);
};
