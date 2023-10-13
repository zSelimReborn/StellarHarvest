// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Crystal.generated.h"

class UBoxComponent;
class UHighlightComponent;
class UWidgetComponent;
class UInteractWidgetComponent;
class UCrystalClusterWidget;

UCLASS()
class STELLARHARVEST_API ACrystal : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrystal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnFinishCrystals();
	
	void DrawDebugInfo() const;

	void SetupCrystalClusterWidget();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Interact interface
public:
	void StartInteraction_Implementation(AActor* ActorInteracting) override;
	void FinishInteraction_Implementation(AActor* ActorInteracting) override;
	bool CanBeInteracted_Implementation(AActor* ActorInteracting) const override;

public:
	UFUNCTION(BlueprintCallable)
	int32 Harvest(const int32 RequestedCrystals);

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetMaxAmountCrystals() const { return MaxAmountCrystal; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool HasCrystals() const { return CurrentAmountCrystal > 0; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetCrystalsAvailable() const { return CurrentAmountCrystal; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetTimeToHarvest() const { return TimeToHarvest; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentHarvestTime() const { return CurrentHarvestTime; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentHarvestTimeRatio() const { return CurrentHarvestTime / TimeToHarvest; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetRemainingHarvestTime() const { return TimeToHarvest - CurrentHarvestTime; }

	void AddHarvestTime(const float);

// Callbacks
protected:
	UFUNCTION()
	void OnActivateHighlight();

	UFUNCTION()
	void OnDeactivateHighlight();
	
// Properties
protected:
	UPROPERTY(EditAnywhere, Category="Harvest")
	int32 MaxAmountCrystal = 10;

	UPROPERTY(EditAnywhere, Category="Harvest")
	float TimeToHarvest = 5.f;
	
	UPROPERTY(Transient)
	int32 CurrentAmountCrystal = 0;

	UPROPERTY(Transient)
	float CurrentHarvestTime = 0.f;

	UPROPERTY(Transient)
	TObjectPtr<AActor> CurrentHarvestingActor;

	UPROPERTY(Transient)
	bool bSomeoneHarvesting = false;

	UPROPERTY(Transient)
	TObjectPtr<UCrystalClusterWidget> CrystalClusterWidgetRef;
	
// Components
protected:
	UPROPERTY(VisibleAnywhere, NoClear)
	TObjectPtr<USceneComponent> DefaultSceneComponent;

	UPROPERTY(VisibleAnywhere, NoClear)
	TObjectPtr<UStaticMeshComponent> RockMeshComponent;

	UPROPERTY(VisibleAnywhere, NoClear)
	TObjectPtr<UStaticMeshComponent> CrystalMeshComponent;

	UPROPERTY(VisibleAnywhere, NoClear)
	TObjectPtr<UBoxComponent> BoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, NoClear)
	TObjectPtr<UHighlightComponent> HighlightComponent;

	UPROPERTY(VisibleAnywhere, NoClear)
	TObjectPtr<UInteractWidgetComponent> InteractWidgetComponent;

	UPROPERTY(VisibleAnywhere, NoClear)
	TObjectPtr<UWidgetComponent> InfoWidgetComponent;
};
