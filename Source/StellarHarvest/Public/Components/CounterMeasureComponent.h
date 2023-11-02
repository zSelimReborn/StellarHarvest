// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CounterMeasureComponent.generated.h"

class AAIMonsterController;
class UParticleSystem;
class USphereComponent;

UENUM(BlueprintType)
enum class ECounterMeasureState : uint8
{
	ECMS_Idle			UMETA(DisplayName="Idle"),
	ECMS_Active			UMETA(DisplayName="Active"),
	ECMS_Cooldown		UMETA(DisplayName="Cooling down"),
	ECMS_MAX
};

UENUM(BlueprintType)
enum class EStunDealMode : uint8
{
	ESDM_Instant	UMETA(DisplayName="Instant"),
	ESDM_Gradual	UMETA(DisplayName="Gradually")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUseCounterMeasureDelegate, const int32, CurrentAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemiesInRangeDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoseEnemiesInRangeDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCooldownFinishedDelegate);
// BAD NAMING
// Delegate called when counter measure finish dealing stun
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishDelegate);

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

	void ApplyStun(AActor* Target) const;

	void ApplyStunToAll();

	void UpdateActive(const float DeltaTime);
	
	void UpdateCooldown(const float DeltaTime);

	void UpdateGradualStun(const float DeltaTime);

	void CheckForNotStunned();
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetTotalAmount() const { return TotalAmount; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetCurrentAmount() const { return CurrentAmount; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool HasCounterMeasures() const { return CurrentAmount > 0; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetDuration() const { return Duration; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentDuration() const { return CurrentDuration; }

	UFUNCTION(BlueprintCallable)
	void UseCounterMeasure();

	UFUNCTION(BlueprintCallable)
	void SetTriggerVolume(USphereComponent* Volume);

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCooldownDuration() const { return CooldownDuration; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCooldownCurrentTime() const { return CooldownCurrentTime; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCooldownCurrentRatio() const { return CooldownCurrentTime / CooldownDuration; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsCoolingDown() const { return State == ECounterMeasureState::ECMS_Cooldown; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE ECounterMeasureState GetState() const { return State; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool HasEnemies() const { return Enemies.Num() > 0; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE EStunDealMode GetStunMode() const { return StunMode; }

// Events
public:
	FOnUseCounterMeasureDelegate& OnUseCounterMeasure() { return OnUseCounterMeasureDelegate; }

	FOnEnemiesInRangeDelegate& OnEnemiesInRange() { return OnEnemiesInRangeDelegate; }

	FOnLoseEnemiesInRangeDelegate& OnLoseEnemiesInRange() { return OnLoseEnemiesInRangeDelegate; }

	FOnCooldownFinishedDelegate& OnCooldownFinished() { return OnCooldownFinishedDelegate; }

	FOnFinishDelegate& OnFinish() { return OnFinishDelegate; }
	
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
	float Duration = 3.f;

	UPROPERTY(Transient)
	float CurrentDuration = 0.f;

	UPROPERTY(EditAnywhere, Category="CounterMeasure")
	EStunDealMode StunMode = EStunDealMode::ESDM_Instant;

	UPROPERTY(Transient)
	float GradualCurrentRadius = 0.f;
	
	UPROPERTY(EditAnywhere, Category="VFX")
	TObjectPtr<UParticleSystem> HitParticle;

	UPROPERTY(EditAnywhere, Category="Stun")
	float StunDuration = 5.f;

	UPROPERTY(EditAnywhere, Category="Stun")
	float StunRandomDeviation = 1.f;

	UPROPERTY(EditAnywhere, Category="Cooldown")
	float CooldownDuration = 10.f;

	UPROPERTY(Transient)
	float CooldownCurrentTime = 0.f;
	
	UPROPERTY(Transient)
	ECounterMeasureState State = ECounterMeasureState::ECMS_Idle;
	
	UPROPERTY(Transient)
	TObjectPtr<USphereComponent> TriggerVolumeRef;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AActor>> Enemies;

	UPROPERTY(Transient)
	FVector FireCounterMeasureLocation;

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

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnFinishDelegate OnFinishDelegate;
};
