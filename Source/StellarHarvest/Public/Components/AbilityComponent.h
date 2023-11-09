// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"

class UEffectAbility;

UENUM(BlueprintType)
enum class EAbilityState : uint8
{
	EAS_Idle		UMETA(DisplayName="Idle"),
	EAS_Active		UMETA(DisplayName="Active"),
	EAS_Cooldown	UMETA(DisplayName="Cooldown"),
	EAS_MAX
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPrimaryAbilityAppliedDelegate, const UEffectAbility*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPrimaryAbilityRemovedDelegate, const UEffectAbility*, Ability);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSecondaryAbilityAppliedDelegate, const UEffectAbility*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSecondaryAbilityRemovedDelegate, const UEffectAbility*, Ability);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPrimaryAbilityAvailableDelegate, const UEffectAbility*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSecondaryAbilityAvailableDelegate, const UEffectAbility*, Ability);

// Now handles only UEffectAbility
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, ToolTip="Component to apply effect ability to owner") )
class STELLARHARVEST_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAbilityComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ApplyPrimaryAbility();

	UFUNCTION(BlueprintCallable)
	void ApplySecondaryAbility();

// Helpers
protected:
	void ApplyAbility(UEffectAbility* Ability);

	void UpdateActiveAbility(const float DeltaTime);

	void ResetLastAbility();

	void Cooldown(const float DeltaTime);

	void DrawDebugInfo() const;
	
// Getters
public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE UEffectAbility* GetPrimaryAbility() const { return PrimaryAbility; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UEffectAbility* GetSecondaryAbility() const { return SecondaryAbility; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UEffectAbility* GetLastAbility() const { return LastAbility; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE EAbilityState GetPrimaryAbilityState() const { return PrimaryAbilityState; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE EAbilityState GetSecondaryAbilityState() const { return SecondaryAbilityState; }
	
	UFUNCTION(BlueprintPure)
	bool CanApplyPrimaryAbility() const;

	UFUNCTION(BlueprintPure)
	bool CanApplySecondaryAbility() const;

// Properties
protected:
	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<UEffectAbility> PrimaryAbilityClass;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<UEffectAbility> SecondaryAbilityClass;

	UPROPERTY(BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UEffectAbility> PrimaryAbility;

	UPROPERTY(BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UEffectAbility> SecondaryAbility;

	UPROPERTY(Transient, BlueprintReadOnly)
	EAbilityState PrimaryAbilityState = EAbilityState::EAS_Idle;

	UPROPERTY(Transient, BlueprintReadOnly)
	EAbilityState SecondaryAbilityState = EAbilityState::EAS_Idle;
	
	UPROPERTY(Transient)
	TObjectPtr<UEffectAbility> LastAbility;

	UPROPERTY(Transient)
	float LastAbilityCurrentTime = 0.f;

	UPROPERTY(Transient)
	float PrimaryCooldownTime = 0.f;

	UPROPERTY()
	float SecondaryCooldownTime = 0.f;

// Events
protected:
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnPrimaryAbilityAppliedDelegate OnPrimaryAbilityApplied;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnPrimaryAbilityRemovedDelegate OnPrimaryAbilityRemoved;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnPrimaryAbilityAvailableDelegate OnPrimaryAbilityAvailable;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnSecondaryAbilityAppliedDelegate OnSecondaryAbilityApplied;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnSecondaryAbilityRemovedDelegate OnSecondaryAbilityRemoved;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnSecondaryAbilityAvailableDelegate OnSecondaryAbilityAvailable;
};
