// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CrystalCollectorComponent.h"

#include "Components/TransformAnimComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Gameplay/Interact/Crystal.h"

// Sets default values for this component's properties
UCrystalCollectorComponent::UCrystalCollectorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UCrystalCollectorComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}

void UCrystalCollectorComponent::AddCrystals(const int32 NewCrystals)
{
	CrystalsCollected += NewCrystals;
	UE_LOG(LogTemp, Error, TEXT("%s has %d crystals"), *GetOwner()->GetActorLabel(), CrystalsCollected);
	// TODO Dispatch events
}

void UCrystalCollectorComponent::TickHarvest(const float DeltaTime)
{
	if (CurrentCrystalGather != nullptr && !bOneShotHarvest)
	{
		CurrentHarvestTime = FMath::Clamp(CurrentHarvestTime + DeltaTime, 0.f, HarvestInterval);
		if (CurrentHarvestTime >= HarvestInterval)
		{
			SingleStepHarvest();
			CurrentHarvestTime = 0.f;
		}
	}
}

void UCrystalCollectorComponent::SetupTickHarvest()
{
	if (CurrentCrystalGather == nullptr)
	{
		return;
	}
	
	int32 NumOfPortions = FMath::CeilToInt32<float>(CurrentCrystalGather->GetMaxAmountCrystals() / NumOfCrystalPerInterval);
	// If it's not multiple, add an extra portion
	if (CurrentCrystalGather->GetMaxAmountCrystals() % NumOfCrystalPerInterval != 0)
	{
		NumOfPortions += 1;
	}
		
	HarvestInterval = CurrentCrystalGather->GetTimeToHarvest() / NumOfPortions;
	const int32 CurrentPortion = CurrentCrystalGather->GetCurrentHarvestTime() / HarvestInterval;
	CurrentHarvestTime = CurrentCrystalGather->GetCurrentHarvestTime() - (CurrentPortion * HarvestInterval);
	SetComponentTickEnabled(true);
}

void UCrystalCollectorComponent::FireAnimation() const
{
	UTransformAnimComponent* AnimComponent = GetOwner()->FindComponentByClass<UTransformAnimComponent>();
	if (AnimComponent)
	{
		AnimComponent->StartAnimation();
	}
}

void UCrystalCollectorComponent::StopAnimation() const
{
	UTransformAnimComponent* AnimComponent = GetOwner()->FindComponentByClass<UTransformAnimComponent>();
	if (AnimComponent)
	{
		AnimComponent->StopAnimation();
	}
}

void UCrystalCollectorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TickHarvest(DeltaTime);
}

void UCrystalCollectorComponent::StartHarvesting(ACrystal* CrystalGather)
{
	if (CrystalGather == nullptr)
	{
		return;
	}

	CurrentCrystalGather = CrystalGather;
	if (bDisableMovementDuringHarvest)
	{
		DisableOwnerMovement();
	}

	const float RemainingHarvestTime = CrystalGather->GetRemainingHarvestTime();
	if (bOneShotHarvest)
	{
		GetOwner()->GetWorldTimerManager().SetTimer(HarvestCrystalTimerHandle, this, &UCrystalCollectorComponent::FullHarvest, RemainingHarvestTime, false);
	}
	else
	{
		SetupTickHarvest();
	}

	if (bFireAnimationDuringHarvest)
	{
		FireAnimation();
	}
}

void UCrystalCollectorComponent::StopHarvesting()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(HarvestCrystalTimerHandle);
	CurrentCrystalGather = nullptr;
	HarvestInterval = 0.f;
	CurrentHarvestTime = 0.f;
	EnableOwnerMovement();
	SetComponentTickEnabled(false);
	
	if (bFireAnimationDuringHarvest)
	{
		StopAnimation();
	}
}

void UCrystalCollectorComponent::FullHarvest()
{
	if (CurrentCrystalGather == nullptr)
	{
		return;
	}
	
	const int32 NewCrystals = CurrentCrystalGather->Harvest(CurrentCrystalGather->GetCrystalsAvailable());
	AddCrystals(NewCrystals);
}

void UCrystalCollectorComponent::SingleStepHarvest()
{
	if (CurrentCrystalGather == nullptr)
	{
		return;
	}

	const int32 NewCrystals = CurrentCrystalGather->Harvest(NumOfCrystalPerInterval);
	AddCrystals(NewCrystals);
}

void UCrystalCollectorComponent::EnableOwnerMovement()
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (PawnOwner)
	{
		PawnOwner->GetMovementComponent()->SetActive(true);
	}
}

void UCrystalCollectorComponent::DisableOwnerMovement()
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (PawnOwner)
	{
		PawnOwner->GetMovementComponent()->SetActive(false);
	}
}

