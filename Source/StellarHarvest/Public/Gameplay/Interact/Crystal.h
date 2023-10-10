// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Crystal.generated.h"

class UBoxComponent;
class UHighlightComponent;
class UWidgetComponent;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Interact interface
public:
	void StartInteraction_Implementation(AActor* ActorInteracting) override;
	void FinishInteraction_Implementation(AActor* ActorInteracting) override;

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
	TObjectPtr<UWidgetComponent> InteractWidgetComponent;
};
