// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStates/StellarHarvestGameState.h"

#include "Controllers/TractorPlayerController.h"
#include "GameModes/StellarHarvestGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void AStellarHarvestGameState::BeginPlay()
{
	Super::BeginPlay();
}

void AStellarHarvestGameState::ReceivedGameModeClass()
{
	Super::ReceivedGameModeClass();

	GameModeRef = Cast<AStellarHarvestGameModeBase>(AuthorityGameMode);
}

void AStellarHarvestGameState::StartGame()
{
	if (GameModeRef == nullptr)
	{
		return;
	}
	
	ScoreGoal = GameModeRef->GetCrystalsGoal();
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ATractorPlayerController* PC = Cast<ATractorPlayerController>(Iterator->Get());
		if (PC != nullptr)
		{
			PC->StartGame();
			PC->NewScoreGoal();
		}
	}
}

void AStellarHarvestGameState::RestartGame(APlayerController* PlayerController)
{
	if (PlayerController == nullptr)
	{
		return;
	}
	
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	
	const FInputModeGameOnly InputModeGameOnly;
	PlayerController->SetInputMode(InputModeGameOnly);
	PlayerController->SetShowMouseCursor(false);
	PlayerController->RestartLevel();
}

void AStellarHarvestGameState::GameOver(APlayerController* PlayerController, const bool bIsWinner)
{
	if (PlayerController == nullptr)
	{
		return;
	}

	UGameplayStatics::SetGamePaused(GetWorld(), true);
	if (bIsWinner)
	{
		OnWin(PlayerController);
	}
	else
	{
		OnLose(PlayerController);	
	}
}

void AStellarHarvestGameState::AddScore(APlayerController* PlayerController, const int32 Score)
{
	CurrentScore += Score;
	if (CurrentScore >= ScoreGoal)
	{
		if (ATractorPlayerController* PC = Cast<ATractorPlayerController>(PlayerController))
		{
			PC->ReachedScoreGoal();
		}
	}
}

void AStellarHarvestGameState::OnReturnToBase(APlayerController* PlayerController)
{
	if (PlayerController == nullptr)
	{
		return;
	}

	if (CurrentScore >= ScoreGoal)
	{
		// No check on PlayerController. We're singleplayer
		GameOver(PlayerController, true);
	}
}

void AStellarHarvestGameState::OnWin_Implementation(APlayerController* PlayerController)
{
	ATractorPlayerController* PC = Cast<ATractorPlayerController>(PlayerController);
	if (PC != nullptr)
	{
		PC->GameOver(true);
	}
}

void AStellarHarvestGameState::OnLose_Implementation(APlayerController* PlayerController)
{
	ATractorPlayerController* PC = Cast<ATractorPlayerController>(PlayerController);
	if (PC != nullptr)
	{
		PC->GameOver(false);
	}
}
