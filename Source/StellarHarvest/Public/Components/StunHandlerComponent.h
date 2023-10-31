// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StunHandlerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusApplied, const float, ActualDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatusRemoved);

// TODO This should be made more generic to handle more statuses. 
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STELLARHARVEST_API UStunHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStunHandlerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnRegister() override;
	
	void UpdateStatus(const float);

	void OnStatusFinish();

	void ApplyStun();

	void RemoveStun();
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Apply(const float DeltaDuration);

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsUnderEffect() const { return bIsUnderEffect; }
	
// Properties
protected:
	UPROPERTY(EditAnywhere, Category="Duration")
	float BaseDuration = 20.f;
	
	UPROPERTY(Transient)
	float Duration = BaseDuration;
	
	UPROPERTY(Transient)
	float CurrentTime = 0.f;

	UPROPERTY(Transient)
	bool bIsUnderEffect = false;

	UPROPERTY(Transient)
	TObjectPtr<APawn> OwnerPawn;

// Events
protected:
	UPROPERTY(BlueprintAssignable)
	FOnStatusApplied OnStatusApplied;

	UPROPERTY(BlueprintAssignable)
	FOnStatusRemoved OnStatusRemoved;
};
