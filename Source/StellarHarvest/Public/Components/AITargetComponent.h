// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AITargetComponent.generated.h"

class UAISense;
struct FAIStimulus;
class AAIController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STELLARHARVEST_API UAITargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAITargetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnRegister() override;

// Helpers
protected:
	void SetupPerception();

	bool IsFriendly(const AActor* TestingActor) const;

	void OnSightStimulus(AActor* SourceActor, const FAIStimulus& Stimulus);
	
	void OnHearStimulus(AActor* SourceActor, const FAIStimulus& Stimulus);

	void TrackTarget() const;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartSearch();

	UFUNCTION(BlueprintCallable)
	void StopSearch();

// Callbacks
protected:
	UFUNCTION()
	void OnPerception(AActor* Actor, FAIStimulus Stimulus);

// Properties
protected:
	UPROPERTY(EditAnywhere, Category="Sense Config")
	TSubclassOf<UAISense> SightSense;

	UPROPERTY(EditAnywhere, Category="Sense Config")
	TSubclassOf<UAISense> HearSense;

	UPROPERTY(EditAnywhere, Category="Targeting")
	FName HasTargetBlackboardKey;

	UPROPERTY(EditAnywhere, Category="Targeting")
	FName TargetLocationBlackboardKey;

	UPROPERTY(EditAnywhere, Category="Targeting")
	FName LastKnownTargetLocationBlackboardKey;
	
	UPROPERTY(EditAnywhere, Category="Targeting")
	FName TargetDistanceBlackboardKey;

	UPROPERTY(EditAnywhere, Category="Targeting")
	bool bCheckIfVisibleOnScreen = false;

	UPROPERTY(EditAnywhere, Category="Patrolling")
	FName CanPatrolBlackboardKey;

	UPROPERTY(EditAnywhere, Category="Patrolling")
	FName PatrolLocationBlackboardKey;

	UPROPERTY(Transient)
	TObjectPtr<AAIController> OwnerController;

	UPROPERTY(Transient)
	bool bIsTrackingTarget = false;

	UPROPERTY(Transient)
	TObjectPtr<AActor> TargetRef = nullptr;
};
