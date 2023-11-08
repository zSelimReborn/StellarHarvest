// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AISightTargetInterface.h"
#include "StellarBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USearchInteractableComponent;

UCLASS()
class STELLARHARVEST_API AStellarBaseCharacter : public ACharacter, public IAISightTargetInterface
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

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual UAISense_Sight::EVisibilityResult CanBeSeenFrom(const FCanBeSeenFromContext& Context, FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested, float& OutSightStrength, int32* UserData, const FOnPendingVisibilityQueryProcessedDelegate* Delegate) override;

	UFUNCTION(BlueprintCallable)
	void SetVisible();

	UFUNCTION(BlueprintCallable)
	void SetInvisible();

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsSightVisible() const { return bIsSightVisible; }

// Movement interface
public:
	void RequestMove(const FVector2D&);
	void RequestMove(const FVector&, const float);
	
	void RequestStartInteraction();
	void RequestFinishInteraction();
	
	void RequestSprint() const;
	void RequestWalk() const;
	
// Helpers
protected:
	FRotator GetActualViewRotation() const;
	void InitializeObservers();
	
// Components
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY()
	TObjectPtr<USearchInteractableComponent> SearchItemsComponent;

// Properties
protected:
	UPROPERTY(EditAnywhere, Category="Movement")
	float SprintSpeed = 2000.f;

	UPROPERTY(Transient)
	float InitialWalkSpeed;

	UPROPERTY(BlueprintReadWrite, Transient)
	bool bIsSightVisible = true;
};
