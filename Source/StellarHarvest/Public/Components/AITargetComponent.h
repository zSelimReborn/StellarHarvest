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
};
