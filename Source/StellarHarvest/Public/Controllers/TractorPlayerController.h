// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TractorPlayerController.generated.h"

class ATractorPawn;
class AStellarBaseCharacter;
class UInputAction;
class UInputMappingContext;
struct FInputActionInstance;
class UTractorHud;
class UEffectAbility;

/**
 * 
 */
UCLASS()
class STELLARHARVEST_API ATractorPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

	void RequestAnalogMove(const FInputActionInstance&);
	void RequestMovementUsingMouse(const FInputActionInstance&);
	void RequestStartInteraction(const FInputActionInstance&);
	void RequestFinishInteraction(const FInputActionInstance&);
	void RequestCounterMeasure(const FInputActionInstance&);
	void RequestPrimaryAbility(const FInputActionInstance&);
	void RequestSecondaryAbility(const FInputActionInstance&);
	
	void EnableMouseMove();
	void DisableMouseMove();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsUsingMouseMove() const { return bUseMouseMove && bIsUsingMouseMove; }

	UFUNCTION(BlueprintCallable)
	void StartGame();

	void NewScoreGoal() const;
	
	UFUNCTION(BlueprintCallable)
	void ReachedScoreGoal();

	UFUNCTION(BlueprintCallable)
	void GameOver(const bool bIsWinner);
	
protected:
	void InitializeMappingContext();
	void InitializeHUD();
	float CalculateMouseMovementRatio() const;
	void MoveUsingMouse();
	void SetupEvents();

// Callbacks
protected:
	UFUNCTION()
	void OnCollectCrystals(const int32 CollectedCrystals, const int32 TotalCrystals);

	UFUNCTION()
	void OnUseCounterMeasure(const int32 CurrentAmount);

	UFUNCTION()
	void OnEnemyInRange();

	UFUNCTION()
	void OnLoseEnemiesInRange();

	UFUNCTION()
	void OnCounterMeasureCooldownFinished();

	UFUNCTION()
	void OnPrimaryAbilityApplied(const UEffectAbility* Ability);

	UFUNCTION()
	void OnPrimaryAbilityRemoved(const UEffectAbility* Ability);
	
	UFUNCTION()
	void OnPrimaryAbilityAvailable(const UEffectAbility* Ability);
	
	UFUNCTION()
	void OnSecondaryAbilityApplied(const UEffectAbility* Ability);

	UFUNCTION()
	void OnSecondaryAbilityRemoved(const UEffectAbility* Ability);

	UFUNCTION()
	void OnSecondaryAbilityAvailable(const UEffectAbility* Ability);
	
// Properties
protected:
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	bool bUseMouseMove = false;

	UPROPERTY(VisibleAnywhere, Category="Movement")
	bool bIsUsingMouseMove = bUseMouseMove;

	UPROPERTY(EditAnywhere, Category="Movement")
	float MinMouseDistance = 100.f;

	UPROPERTY(EditAnywhere, Category="Movement")
	bool bScaleMovementBasedOnDistance = true;

	UPROPERTY(Transient)
	float MinMouseDistanceSquared;
	
	UPROPERTY(Transient)
	TObjectPtr<AStellarBaseCharacter> TractorRef;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UTractorHud> HUDWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<UTractorHud> HUDWidgetRef;

// Actions
protected:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> AnalogMoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> RequestMouseMoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> RequestInteractAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> RequestCounterMeasureAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> RequestPrimaryAbilityAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> RequestSecondaryAbilityAction;
};
