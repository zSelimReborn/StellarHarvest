// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AIPatrolMovementComponent.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gameplay/AI/PatrolPath.h"

UAIPatrolMovementComponent::UAIPatrolMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAIPatrolMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
	{
		Stop();
	}

	PawnController = OwnerPawn->GetController<AAIController>();
	bStartOnBeginPlay ? Resume() : Stop();
}

void UAIPatrolMovementComponent::Move(const float DeltaTime)
{
	ensure(PawnController != nullptr);
	
	if (!bIsRunning)
	{
		return;
	}

	if (PatrolPath == nullptr)
	{
		Stop();
		return;
	}

	CurrentPathTime += DeltaTime;
	if (CurrentPathTime > PatrolPath->GetPathDuration())
	{
		CurrentPathTime = 0.f;
	}
	
	const FVector MoveLocation = GetNextLocation();
	PawnController->GetBlackboardComponent()->SetValueAsVector(PatrolLocationBlackboardKey, MoveLocation);
}

void UAIPatrolMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Move(DeltaTime);
}

void UAIPatrolMovementComponent::SetPatrolPath(APatrolPath* NewPath)
{
	PatrolPath = NewPath;
}

void UAIPatrolMovementComponent::Run()
{
	ensure(PawnController != nullptr);
	
	bIsRunning = true;
	SetComponentTickEnabled(true);
	PawnController->GetBlackboardComponent()->SetValueAsBool(IsPatrollingBlackboardKey, true);
}

void UAIPatrolMovementComponent::Stop()
{
	bIsRunning = false;
	SetComponentTickEnabled(false);
	PawnController->GetBlackboardComponent()->SetValueAsBool(IsPatrollingBlackboardKey, false);
}

void UAIPatrolMovementComponent::Resume()
{
	if (PatrolPath == nullptr)
	{
		Stop();
		return;
	}

	FVector NextLocation;
	PatrolPath->GetClosestTimeAndLocation(GetOwner()->GetActorLocation(), CurrentPathTime, NextLocation);
	Run();
}

FVector UAIPatrolMovementComponent::GetNextLocation() const
{
	if (PatrolPath == nullptr)
	{
		return FVector::ZeroVector;
	}

	return PatrolPath->GetPathLocationAtTime(CurrentPathTime);
}

FVector UAIPatrolMovementComponent::GetNearestLocationOnPath() const
{
	if (PatrolPath == nullptr)
	{
		return FVector::ZeroVector;
	}

	return PatrolPath->GetClosestLocation(GetOwner()->GetActorLocation());
}

