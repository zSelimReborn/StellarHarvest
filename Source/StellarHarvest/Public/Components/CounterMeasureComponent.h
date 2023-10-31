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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCooldownFinishedDelegate);

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

	void UpdateCooldown(const float DeltaTime);
	
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

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCooldownDuration() const { return CooldownDuration; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCooldownCurrentTime() const { return CooldownCurrentTime; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCooldownCurrentRatio() const { return CooldownCurrentTime / CooldownDuration; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsCoolingDown() const { return !bIsUsable; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool HasEnemies() const { return Enemies.Num() > 0; }

// Events
public:
	FOnUseCounterMeasureDelegate& OnUseCounterMeasure() { return OnUseCounterMeasureDelegate; }

	FOnEnemiesInRangeDelegate& OnEnemiesInRange() { return OnEnemiesInRangeDelegate; }

	FOnLoseEnemiesInRangeDelegate& OnLoseEnemiesInRange() { return OnLoseEnemiesInRangeDelegate; }

	FOnCooldownFinishedDelegate& OnCooldownFinished() { return OnCooldownFinishedDelegate; }
	
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

	UPROPERTY(EditAnywhere, Category="Cooldown")
	float CooldownDuration = 10.f;

	UPROPERTY(Transient)
	float CooldownCurrentTime = 0.f;

	UPROPERTY(Transient)
	bool bIsUsable = true;
	
	UPROPERTY(Transient)
	TObjectPtr<UShapeComponent> TriggerVolumeRef;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AActor>> Enemies;

// Delegates
protected:
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnUseCounterMeasureDelegate OnUseCounterMeasureDelegate;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnEnemiesInRangeDelegate OnEnemiesInRangeDelegate;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnLoseEnemiesInRangeDelegate OnLoseEnemiesInRangeDelegate;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnCooldownFinishedDelegate OnCooldownFinishedDelegate;
};
