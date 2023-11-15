// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StellarHarvestGameState.generated.h"

/**
 *  GameState created to be compliant to unreal engine networking system
 *  Handles game score, notify player next objective and win/lose 
 */
UCLASS()
class STELLARHARVEST_API AStellarHarvestGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void GameOver(APlayerController* PlayerController, const bool bIsWinner);

	UFUNCTION(BlueprintCallable)
	void AddScore(APlayerController* PlayerController, const int32 Score);

	UFUNCTION(BlueprintNativeEvent)
	void OnLose(APlayerController* PlayerController);

	UFUNCTION(BlueprintNativeEvent)
	void OnWin(APlayerController* PlayerController);

	UFUNCTION(BlueprintPure)
	int32 GetCurrentScore() const { return CurrentScore; }

	UFUNCTION(BlueprintPure)
	int32 GetScoreGoal() const { return ScoreGoal; }

protected:
	UPROPERTY(Transient)
	int32 CurrentScore = 0;

	UPROPERTY(Transient)
	int32 ScoreGoal = 0;
};
