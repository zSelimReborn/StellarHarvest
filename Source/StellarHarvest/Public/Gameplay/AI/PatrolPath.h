// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

class USplineComponent;

UCLASS()
class STELLARHARVEST_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetPathLength() const;

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetPathDuration() const;

	UFUNCTION(BlueprintPure)
	FORCEINLINE USplineComponent* GetPath() const;

	UFUNCTION(BlueprintPure)
	FVector GetPathLocationAtTime(const float Time) const;

	UFUNCTION(BlueprintPure)
	FVector GetClosestLocation(const FVector& Location) const;

	UFUNCTION(BlueprintPure)
	void GetClosestTimeAndLocation(const FVector& Location, float& OutTime, FVector& LocationOnSpline) const;
	
// Components
protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<USplineComponent> PathComponent;
};
