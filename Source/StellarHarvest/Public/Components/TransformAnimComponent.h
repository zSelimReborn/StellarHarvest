// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TransformAnimComponent.generated.h"

UENUM(BlueprintType)
enum class ETractorAnimState : uint8
{
	ETAS_Default	UMETA(DisplayName="Default"),
	ETAS_Playing	UMETA(DisplayName="Playing"),
	ETAS_Reverse	UMETA(DisplayName="Reverse"),
	ETAS_Stop		UMETA(DisplayName="Stop"),
	ETAS_MAX		UMETA(DisplayName="MAX")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, ToolTip="Loops a transform animation for actors."))
class STELLARHARVEST_API UTransformAnimComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTransformAnimComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Public interface
public:
	UFUNCTION(BlueprintCallable)
	void SetUpdatedComponent(USceneComponent* NewUpdatedComponent);

	UFUNCTION(BlueprintCallable)
	void StartAnimation();

	UFUNCTION(BlueprintCallable)
	void StopAnimation();

	UFUNCTION(BlueprintCallable)
	void ReverseAnimation();

	UFUNCTION(BlueprintPure)
	FORCEINLINE ETractorAnimState GetAnimState() const { return AnimState; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsGoingForward() const { return AnimState == ETractorAnimState::ETAS_Playing; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsGoingBackward() const { return AnimState == ETractorAnimState::ETAS_Reverse || AnimState == ETractorAnimState::ETAS_Stop; }

// Helpers
protected:
	void TickAnimation(const float);
	void OnStopAnimation();
	
// Properties
protected:
	UPROPERTY(Transient)
	TObjectPtr<USceneComponent> UpdatedComponent;

	UPROPERTY(EditAnywhere, Category="Animation")
	bool bAnimateLocation = true;
	
	UPROPERTY(EditAnywhere, Category="Animation", meta=(EditCondition=bAnimateLocation))
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category="Animation")
	bool bAnimateRotation = true;
	
	UPROPERTY(EditAnywhere, Category="Animation", meta=(EditCondition=bAnimateRotation))
	FRotator TargetRotation = FRotator::ZeroRotator;

	UPROPERTY(VisibleAnywhere, Category="Animation")
	FVector InitialLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Category="Animation")
	FRotator InitialRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category="Animation")
	float AnimationTime = 5.f;

	UPROPERTY()
	float CurrentAnimationTime = 0.f;

	UPROPERTY(EditAnywhere, Category="Animation")
	bool bLoopAnimation = true;

	UPROPERTY(VisibleAnywhere, Category="Animation")
	ETractorAnimState AnimState = ETractorAnimState::ETAS_Default;
};
