// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/AI/PatrolPath.h"

#include "Components/SplineComponent.h"

APatrolPath::APatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Root"));
	SetRootComponent(DefaultSceneRoot);

	PathComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Path Component"));
	PathComponent->SetupAttachment(DefaultSceneRoot);
}

void APatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
}

float APatrolPath::GetPathLength() const
{
	return PathComponent->GetSplineLength();
}

float APatrolPath::GetPathDuration() const
{
	return PathComponent->Duration;
}

USplineComponent* APatrolPath::GetPath() const
{
	return PathComponent;
}

FVector APatrolPath::GetPathLocationAtTime(const float Time) const
{
	return PathComponent->GetLocationAtTime(Time, ESplineCoordinateSpace::World);
}

FVector APatrolPath::GetClosestLocation(const FVector& Location) const
{
	return PathComponent->FindLocationClosestToWorldLocation(Location, ESplineCoordinateSpace::World);
}

void APatrolPath::GetClosestTimeAndLocation(const FVector& Location, float& OutTime, FVector& LocationOnSpline) const
{
	LocationOnSpline = GetClosestLocation(Location);
	const float DistanceOnSpline = PathComponent->GetDistanceAlongSplineAtLocation(LocationOnSpline, ESplineCoordinateSpace::World);
	OutTime = PathComponent->GetTimeAtDistanceAlongSpline(DistanceOnSpline);
}

