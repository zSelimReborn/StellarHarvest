// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIPatrolMovementComponent.generated.h"

class APatrolPath;
class AAIController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STELLARHARVEST_API UAIPatrolMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIPatrolMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Move(const float);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetPatrolPath(APatrolPath* NewPath);

	UFUNCTION(BlueprintCallable)
	void Run();

	UFUNCTION(BlueprintCallable)
	void Stop();

	UFUNCTION(BlueprintCallable)
	void Resume();

	UFUNCTION(BlueprintPure)
	FVector GetNextLocation() const;

	UFUNCTION(BlueprintCallable)
	FVector GetNearestLocationOnPath() const;
	
// Properties
protected:
	UPROPERTY(EditAnywhere, Category="Path")
	TObjectPtr<APatrolPath> PatrolPath;

	UPROPERTY(EditAnywhere, Category="Path")
	bool bStartOnBeginPlay = true;

	UPROPERTY(EditAnywhere, Category="AI")
	FName IsPatrollingBlackboardKey;

	UPROPERTY(EditAnywhere, Category="AI")
	FName PatrolLocationBlackboardKey;

	UPROPERTY(Transient)
	bool bIsRunning = false;

	UPROPERTY(Transient)
	float CurrentPathTime = 0.f;

	UPROPERTY(Transient)
	TObjectPtr<AAIController> PawnController;
};
