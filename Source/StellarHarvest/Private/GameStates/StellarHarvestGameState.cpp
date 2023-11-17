// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStates/StellarHarvestGameState.h"

#include "Controllers/TractorPlayerController.h"
#include "GameModes/StellarHarvestGameModeBase.h"
#include "GameStates/StellarHarvestSaveGame.h"
#include "Kismet/GameplayStatics.h"

AStellarHarvestGameState::AStellarHarvestGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

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
	bUseTimer = GameModeRef->ShouldUseTimer();
	LoadSaveGame();
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ATractorPlayerController* PC = Cast<ATractorPlayerController>(Iterator->Get());
		if (PC != nullptr)
		{
			PC->StartGame();
			PC->NewScoreGoal();
		}
	}

	bGameStarted = true;
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

float AStellarHarvestGameState::GetBestTime() const
{
	if (SaveGameRef == nullptr)
	{
		return -1.f;
	}

	return SaveGameRef->GetBestTime();
}

void AStellarHarvestGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateTimer(DeltaSeconds);
}

void AStellarHarvestGameState::UpdateTimer(const float DeltaTime)
{
	if (bUseTimer && bGameStarted)
	{
		CurrentTimer += DeltaTime;
	}
}

void AStellarHarvestGameState::TrySaveGame()
{
	if (SaveGameRef == nullptr)
	{
		SaveGameRef = GetDefaultSaveGame();
	}

	LastBestTime = SaveGameRef->GetBestTime();
	
	if (SaveGameRef->GetBestTime() <= 0.f || CurrentTimer < SaveGameRef->GetBestTime())
	{
		SaveGameRef->SetBestTime(CurrentTimer);
	}

	UGameplayStatics::SaveGameToSlot(SaveGameRef, SaveSlot, 0);
}

void AStellarHarvestGameState::LoadSaveGame()
{
	SaveGameRef = Cast<UStellarHarvestSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, 0));
	if (SaveGameRef == nullptr)
	{
		SaveGameRef = GetDefaultSaveGame();
		UGameplayStatics::SaveGameToSlot(SaveGameRef, SaveSlot, 0);
	}

	LastBestTime = SaveGameRef->GetBestTime();
}

UStellarHarvestSaveGame* AStellarHarvestGameState::GetDefaultSaveGame() const
{
	UStellarHarvestSaveGame* SaveGame = Cast<UStellarHarvestSaveGame>(UGameplayStatics::CreateSaveGameObject(UStellarHarvestSaveGame::StaticClass()));
	SaveGame->SetBestTime(-1.f);
	return SaveGame;
}

void AStellarHarvestGameState::OnWin_Implementation(APlayerController* PlayerController)
{
	ATractorPlayerController* PC = Cast<ATractorPlayerController>(PlayerController);
	if (PC != nullptr)
	{
		TrySaveGame();
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
