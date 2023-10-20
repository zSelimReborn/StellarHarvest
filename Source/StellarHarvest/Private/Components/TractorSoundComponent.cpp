// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TractorSoundComponent.h"

#include "Components/AudioComponent.h"
#include "Perception/AISense_Hearing.h"

UTractorSoundComponent::UTractorSoundComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTractorSoundComponent::BeginPlay()
{
	Super::BeginPlay();
	Setup();
}

void UTractorSoundComponent::Setup()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	SpeedThresholdSquared = SpeedThreshold * SpeedThreshold;
	SetComponentTickEnabled((bShouldGenerateNoise || bSoundOnMove));
}

void UTractorSoundComponent::UpdateEngineSound(const float OwnerSpeed, const float DeltaTime) const
{
	if (!bSoundOnMove || AudioComponent == nullptr)
	{
		return;
	}

	const float CurrentVolume = FMath::GetMappedRangeValueClamped(
		TRange<float>(0.f, SpeedThresholdSquared),
		TRange<float>(DefaultVolume, EngineFullVolume),
		OwnerSpeed
	);

	AudioComponent->SetVolumeMultiplier(CurrentVolume);
}

void UTractorSoundComponent::GenerateNoise(const float OwnerSpeed, const float DeltaTime)
{
	if (!bShouldGenerateNoise)
	{
		return;
	}
	
	CurrentNoiseTime += DeltaTime;
	if (CurrentNoiseTime >= NoiseGenerationTime)
	{
		CurrentNoiseTime = 0.f;
		const float Range = FMath::GetMappedRangeValueClamped(
		TRange<float>(0.f, SpeedThresholdSquared),
		TRange<float>(NoiseMinRange, NoiseMaxRange),
		OwnerSpeed
		);

		UAISense_Hearing::ReportNoiseEvent(
			GetWorld(),
			GetOwner()->GetActorLocation(),
		1.f,
		GetOwner(),
		Range
		);
	}
}

void UTractorSoundComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float CurrentSpeed = GetOwner()->GetVelocity().SquaredLength();
	UpdateEngineSound(CurrentSpeed, DeltaTime);
	GenerateNoise(CurrentSpeed, DeltaTime);
}

