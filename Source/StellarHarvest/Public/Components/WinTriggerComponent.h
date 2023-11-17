// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WinTriggerComponent.generated.h"

class AStellarHarvestGameState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STELLARHARVEST_API UWinTriggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWinTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnRegister() override;

	void BindEvents();

	void UnBindEvents();

	UFUNCTION()
	void OnVolumeStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void TryWinGame(AActor* OverlappingActor) const;
	
public:	
	UFUNCTION(BlueprintCallable)
	void SetTriggerVolume(UShapeComponent* NewVolume);

// Properties
protected:
	UPROPERTY(Transient)
	TObjectPtr<UShapeComponent> TriggerVolume;

	UPROPERTY(Transient)
	TObjectPtr<AStellarHarvestGameState> GameState;
	
};
