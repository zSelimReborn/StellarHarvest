// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStates/StellarHarvestGameState.h"

#include "Controllers/TractorPlayerController.h"
#include "GameModes/StellarHarvestGameModeBase.h"

void AStellarHarvestGameState::BeginPlay()
{
	Super::BeginPlay();

	if (const AStellarHarvestGameModeBase* GameMode = Cast<AStellarHarvestGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		ScoreGoal = GameMode->GetCrystalsGoal();
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			const ATractorPlayerController* PC = Cast<ATractorPlayerController>(Iterator->Get());
			if (PC != nullptr)
			{
				PC->NewScoreGoal();
			}
		}
	}
}

void AStellarHarvestGameState::GameOver(APlayerController* PlayerController, const bool bIsWinner)
{
	if (PlayerController == nullptr)
	{
		return;
	}

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
