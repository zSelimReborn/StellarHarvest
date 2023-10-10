// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StellarBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USearchInteractableComponent;

UCLASS()
class STELLARHARVEST_API AStellarBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AStellarBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

// Movement interface
public:
	void RequestMove(const FVector2D&);
	void RequestMove(const FVector&, const float);
	void RequestStartInteraction();
	void RequestFinishInteraction();
	
// Helpers
protected:
	FRotator GetActualViewRotation() const;
	void InitializeObservers();
	void ToggleHighlightItem(const AActor*, const bool) const;

// Callbacks
protected:
	UFUNCTION()
	void OnInteractableFound(const FHitResult& HitResult, AActor* NewItem);

	UFUNCTION()
	void OnInteractableLost(AActor* LostItem);

// Components
protected:
	UPROPERTY(VisibleAnywhere, NoClear)
	TObjectPtr<UStaticMeshComponent> TractorBodyComponent;

	UPROPERTY(VisibleAnywhere, NoClear)
	TObjectPtr<UStaticMeshComponent> TractorDrillComponent;

	UPROPERTY(VisibleAnywhere, NoClear)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, NoClear)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY()
	TObjectPtr<USearchInteractableComponent> SearchItemsComponent;

// Properties
protected:
	UPROPERTY(Transient)
	TObjectPtr<AActor> SelectedInteractable;
};
