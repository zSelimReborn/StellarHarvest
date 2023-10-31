// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CounterMeasureComponent.generated.h"

class AAIMonsterController;
class UParticleSystem;
class UShapeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUseCounterMeasureDelegate, const int32, CurrentAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemiesInRangeDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoseEnemiesInRangeDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STELLARHARVEST_API UCounterMeasureComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCounterMeasureComponent();

protected:
	virtual void BeginPlay() override;

	virtual void OnRegister() override;

	void BindTriggerVolume();

	void UnBindTriggerVolume();

	void ApplyStun();
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetTotalAmount() const { return TotalAmount; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetCurrentAmount() const { return CurrentAmount; }

	UFUNCTION(BlueprintCallable)
	void UseCounterMeasure();

	UFUNCTION(BlueprintCallable)
	void SetTriggerVolume(UShapeComponent* Volume);

// Events
public:
	FOnUseCounterMeasureDelegate& OnUseCounterMeasure() { return OnUseCounterMeasureDelegate; }

	FOnEnemiesInRangeDelegate& OnEnemiesInRange() { return OnEnemiesInRangeDelegate; }

	FOnLoseEnemiesInRangeDelegate& OnLoseEnemiesInRange() { return OnLoseEnemiesInRangeDelegate; }
	
// Callbacks
protected:
	UFUNCTION()
	void OnVolumeStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnVolumeFinishOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
// Properties
protected:
	UPROPERTY(EditAnywhere, Category="CounterMeasure")
	int32 TotalAmount = 5;

	UPROPERTY(VisibleAnywhere, Category="CounterMeasure")
	int32 CurrentAmount = 0;

	UPROPERTY(EditAnywhere, Category="CounterMeasure")
	TObjectPtr<UParticleSystem> StunParticle;

	UPROPERTY(EditAnywhere, Category="Stun")
	float Duration = 5.f;

	UPROPERTY(EditAnywhere, Category="Stun")
	float RandomDeviation = 1.f;
	
	UPROPERTY(Transient)
	TObjectPtr<UShapeComponent> TriggerVolumeRef;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AActor>> Enemies;

// Delegates
protected:
	FOnUseCounterMeasureDelegate OnUseCounterMeasureDelegate;

	FOnEnemiesInRangeDelegate OnEnemiesInRangeDelegate;

	FOnLoseEnemiesInRangeDelegate OnLoseEnemiesInRangeDelegate;
};
