// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StellarHarvestGameState.generated.h"

class AStellarHarvestGameModeBase;
class UStellarHarvestSaveGame;

/**
 *  GameState created to be compliant to unreal engine networking system
 *  Handles game score, notify player next objective and win/lose 
 */
UCLASS()
class STELLARHARVEST_API AStellarHarvestGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AStellarHarvestGameState();
	
protected:
	virtual void BeginPlay() override;

	virtual void ReceivedGameModeClass() override;

// BP Interface
public:
	UFUNCTION(BlueprintCallable)
	void StartGame();
	
	UFUNCTION(BlueprintCallable)
	void RestartGame(APlayerController* PlayerController);
	
	UFUNCTION(BlueprintCallable)
	void GameOver(APlayerController* PlayerController, const bool bIsWinner);

	UFUNCTION(BlueprintCallable)
	void AddScore(APlayerController* PlayerController, const int32 Score);
	
	UFUNCTION(BlueprintCallable)
	void OnReturnToBase(APlayerController* PlayerController);

	UFUNCTION(BlueprintPure)
	int32 GetCurrentScore() const { return CurrentScore; }

	UFUNCTION(BlueprintPure)
	int32 GetScoreGoal() const { return ScoreGoal; }

	UFUNCTION(BlueprintPure)
	bool ShouldUseTimer() const { return bUseTimer; }

	UFUNCTION(BlueprintPure)
	float GetCurrentTimer() const { return CurrentTimer; }

	UFUNCTION(BlueprintPure)
	float GetBestTime() const;

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetLastBestTime() const { return LastBestTime; };

public:
	virtual void Tick(float DeltaSeconds) override;

// Helpers
protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnLose(APlayerController* PlayerController);

	UFUNCTION(BlueprintNativeEvent)
	void OnWin(APlayerController* PlayerController);

	void UpdateTimer(const float DeltaTime);

	void TrySaveGame();
	
	void LoadSaveGame();

	UStellarHarvestSaveGame* GetDefaultSaveGame() const;
	
// Properties
protected:
	UPROPERTY(Transient)
	int32 CurrentScore = 0;

	UPROPERTY(Transient)
	int32 ScoreGoal = 0;

	UPROPERTY(Transient)
	bool bUseTimer = false;

	UPROPERTY(Transient)
	bool bGameStarted = false;

	UPROPERTY(Transient)
	float CurrentTimer = 0.f;

	UPROPERTY(Transient)
	float LastBestTime = -1.f;

	UPROPERTY(Transient)
	TObjectPtr<AStellarHarvestGameModeBase> GameModeRef;

	UPROPERTY()
	FString SaveSlot = TEXT("StellarHarvestSaveGame");
	
	UPROPERTY()
	TObjectPtr<UStellarHarvestSaveGame> SaveGameRef;
};
