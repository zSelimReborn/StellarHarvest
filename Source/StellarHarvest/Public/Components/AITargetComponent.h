// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AITargetComponent.generated.h"

class UAISense;
struct FAIStimulus;
class AAIController;

UENUM(BlueprintType)
enum class ETargetingState : uint8
{
	ETS_Default			UMETA(DisplayName="Default"),
	ETS_Tracking		UMETA(DisplayName="Tracking Target"),
	ETS_Investigating	UMETA(DisplayName="Investigating"),
	ETS_MAX
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetAcquiredDelegate, AActor*, TargetActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetLostDelegate, AActor*, TargetActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInvestigationStartDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInvestigationStopDelegate);

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

	void TrackTarget();

	void Investigate(const float);

	void StopPatrolling() const;

	void RotateTowardsStimuli(const AActor* StimuliActor) const;

	void DrawDebug(const float DeltaTime);
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartSearch();

	UFUNCTION(BlueprintCallable)
	void StopSearch();

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool HasTarget() const { return TargetRef != nullptr; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE AActor* GetTarget() const { return TargetRef; }

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
    FName TargetObjectBlackboardKey;

	UPROPERTY(EditAnywhere, Category="Targeting")
	FName TargetLocationBlackboardKey;
	
	UPROPERTY(EditAnywhere, Category="Targeting")
	FName TargetDistanceBlackboardKey;
	
	UPROPERTY(EditAnywhere, Category="Investigating")
	FName IsInvestigatingBlackboardKey;

	UPROPERTY(EditAnywhere, Category="Investigating")
	FName InvestigationLocationBlackboardKey;

	UPROPERTY(EditAnywhere, Category="Investigating")
	float MaxTimeInvestigation = 5.f;

	UPROPERTY(EditAnywhere, Category="Targeting")
	bool bRotateTowardsStimuli = true;
	
	UPROPERTY(Transient)
	TObjectPtr<AAIController> OwnerController;

	UPROPERTY(VisibleAnywhere, Category="Targeting")
	ETargetingState State = ETargetingState::ETS_Default;

	UPROPERTY(Transient)
	FVector LastKnownLocation = FVector::ZeroVector;

	UPROPERTY(Transient)
	float CurrentTimeInvestigation = 0.f;

	UPROPERTY(Transient)
	TObjectPtr<AActor> TargetRef = nullptr;

	UPROPERTY(Transient)
	bool bDebugSawSomething = false;

	UPROPERTY(Transient)
	bool bDebugHeardSomething = false;

	UPROPERTY(Transient)
	float CurrentDebugTime = 0.f;

// Events
public:
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnTargetAcquiredDelegate OnTargetAcquired;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnTargetLostDelegate OnTargetLost;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnInvestigationStartDelegate OnInvestigationStart;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnInvestigationStopDelegate OnInvestigationStop;
};
