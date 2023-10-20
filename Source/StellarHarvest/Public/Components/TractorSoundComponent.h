// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TractorSoundComponent.generated.h"

class USoundCue;
class UAudioComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STELLARHARVEST_API UTractorSoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTractorSoundComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Setup();

	void UpdateEngineSound(const float OwnerSpeed, const float DeltaTime) const;

	void GenerateNoise(const float OwnerSpeed, const float DeltaTime);
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Properties
protected:
	UPROPERTY(EditAnywhere, Category="Sound")
	float SpeedThreshold = 500.f;

	UPROPERTY(EditAnywhere, Category="Engine")
	bool bSoundOnMove = true;
	
	UPROPERTY(EditAnywhere, Category="Engine", meta=(ClampMin=0, ClampMax=1))
	float DefaultVolume = 0.4;

	UPROPERTY(EditAnywhere, Category="Engine", meta=(ClampMin=0, ClampMax=1))
	float EngineFullVolume = 0.9;

	UPROPERTY(EditAnywhere, Category="Noise")
	bool bShouldGenerateNoise = true;
	
	UPROPERTY(EditAnywhere, Category="Noise")
	float NoiseMinRange = 200.f;

	UPROPERTY(EditAnywhere, Category="Noise")
	float NoiseMaxRange = 500.f;

	UPROPERTY(EditAnywhere, Category="Noise")
	float NoiseGenerationTime = 1.f;

	UPROPERTY(Transient)
	float SpeedThresholdSquared = 0.f;

	UPROPERTY(Transient)
	float CurrentNoiseTime = 0.f;

	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> AudioComponent;
};
