// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrystalCollectorComponent.generated.h"

class ACrystal;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STELLARHARVEST_API UCrystalCollectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCrystalCollectorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void AddCrystals(const int32 NewCrystals);

	void TickHarvest(const float DeltaTime);

	void SetupTickHarvest();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
// Public interface
public:
	UFUNCTION(BlueprintCallable)
	void StartHarvesting(ACrystal* CrystalGather);

	UFUNCTION(BlueprintCallable)
	void StopHarvesting();

	UFUNCTION(BlueprintCallable)
	void FullHarvest();

	UFUNCTION(BlueprintCallable)
	void SingleStepHarvest();
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetCrystalsCollected() const { return CrystalsCollected; }

	UFUNCTION(BlueprintCallable)
	void EnableOwnerMovement();

	UFUNCTION(BlueprintCallable)
	void DisableOwnerMovement();

// Properties
protected:
	UPROPERTY(VisibleAnywhere, Category="Harvest")
	int32 CrystalsCollected = 0;

	UPROPERTY(EditAnywhere, Category="Harvest")
	bool bOneShotHarvest = false;

	UPROPERTY(EditAnywhere, Category="Harvest", meta=(EditCondition="!bOneShotHarvest"))
	int32 NumOfCrystalPerInterval = 2;
	
	UPROPERTY(Transient)
	float HarvestInterval = 0.f;

	UPROPERTY(Transient)
	float CurrentHarvestTime = 0.f;

	UPROPERTY(EditAnywhere, Category="Harvest")
	bool bDisableMovementDuringHarvest = true;

	UPROPERTY(Transient)
	TObjectPtr<ACrystal> CurrentCrystalGather;

	FTimerHandle HarvestCrystalTimerHandle;
};
