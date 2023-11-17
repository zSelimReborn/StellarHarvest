// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WinTriggerComponent.h"

#include "Components/ShapeComponent.h"
#include "GameStates/StellarHarvestGameState.h"

UWinTriggerComponent::UWinTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWinTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	GameState = Cast<AStellarHarvestGameState>(GetWorld()->GetGameState());
}

void UWinTriggerComponent::OnRegister()
{
	Super::OnRegister();
	UShapeComponent* Volume = GetOwner()->FindComponentByClass<UShapeComponent>();
	SetTriggerVolume(Volume);
}

void UWinTriggerComponent::BindEvents()
{
	if (TriggerVolume == nullptr)
	{
		return;
	}

	TriggerVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &UWinTriggerComponent::OnVolumeStartOverlap);
}

void UWinTriggerComponent::UnBindEvents()
{
	if (TriggerVolume == nullptr)
	{
		return;
	}

	TriggerVolume->OnComponentBeginOverlap.RemoveDynamic(this, &UWinTriggerComponent::OnVolumeStartOverlap);
}

void UWinTriggerComponent::OnVolumeStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Overlapping actor: %s"), *OtherActor->GetActorLabel());
	TryWinGame(OtherActor);	
}

void UWinTriggerComponent::TryWinGame(AActor* OverlappingActor) const
{
	if (GameState == nullptr)
	{
		return;
	}
	
	if (OverlappingActor == nullptr)
	{
		return;
	}

	APawn* Pawn = Cast<APawn>(OverlappingActor);
	if (Pawn == nullptr)
	{
		return;
	}

	APlayerController* PC = Pawn->GetController<APlayerController>();
	if (PC == nullptr)
	{
		return;
	}

	GameState->OnReturnToBase(PC);
}

void UWinTriggerComponent::SetTriggerVolume(UShapeComponent* NewVolume)
{
	if (NewVolume == nullptr)
	{
		return;
	}

	if (NewVolume == TriggerVolume)
	{
		return;
	}

	if (NewVolume->GetOwner() != GetOwner())
	{
		return;
	}

	UnBindEvents();
	TriggerVolume = NewVolume;
	BindEvents();
}

