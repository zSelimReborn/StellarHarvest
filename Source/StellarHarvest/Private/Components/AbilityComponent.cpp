// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AbilityComponent.h"

#include "Gameplay/Abilities/EffectAbility.h"
#include "Interfaces/Ability.h"

static TAutoConsoleVariable<bool> CVarDebugAbilityComponent(
	TEXT("StellarHarvest.Tractor.AbilityComponent"),
	false,
	TEXT("Show debug info about abilities."),
	ECVF_Default
);

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PrimaryAbility = NewObject<UEffectAbility>(GetOwner(), PrimaryAbilityClass);
	SecondaryAbility = NewObject<UEffectAbility>(GetOwner(), SecondaryAbilityClass);
}

void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateActiveAbility(DeltaTime);
	Cooldown(DeltaTime);

	DrawDebugInfo();
}

void UAbilityComponent::ApplyPrimaryAbility()
{
	if (!CanApplyPrimaryAbility())
	{
		return;
	}
	
	ApplyAbility(PrimaryAbility);
	PrimaryAbilityState = EAbilityState::EAS_Active;
}

void UAbilityComponent::ApplySecondaryAbility()
{
	if (!CanApplySecondaryAbility())
	{
		return;
	}

	ApplyAbility(SecondaryAbility);
	SecondaryAbilityState = EAbilityState::EAS_Active;
}

void UAbilityComponent::ApplyAbility(UEffectAbility* Ability)
{
	if (Ability == nullptr)
	{
		return;
	}

	IAbility::Execute_Apply(Ability, GetOwner());
	LastAbility = Ability;
	LastAbilityCurrentTime = 0.f;
}

void UAbilityComponent::UpdateActiveAbility(const float DeltaTime)
{
	if (PrimaryAbilityState != EAbilityState::EAS_Active && SecondaryAbilityState != EAbilityState::EAS_Active)
	{
		return;
	}

	if (LastAbility == nullptr)
	{
		ResetLastAbility();
		return;
	}
	
	LastAbilityCurrentTime += DeltaTime;
	if (LastAbilityCurrentTime >= LastAbility->GetDuration())
	{
		ResetLastAbility();
	}
}

void UAbilityComponent::ResetLastAbility()
{
	IAbility::Execute_Remove(LastAbility, GetOwner());
	PrimaryAbilityState = (PrimaryAbilityState == EAbilityState::EAS_Active)? EAbilityState::EAS_Cooldown : PrimaryAbilityState;
	SecondaryAbilityState = (SecondaryAbilityState == EAbilityState::EAS_Active)? EAbilityState::EAS_Cooldown : SecondaryAbilityState;
	LastAbilityCurrentTime = 0.f;
	LastAbility = nullptr;
}

void UAbilityComponent::Cooldown(const float DeltaTime)
{
	if (PrimaryAbilityState != EAbilityState::EAS_Cooldown && SecondaryAbilityState != EAbilityState::EAS_Cooldown)
	{
		return;
	}

	// TODO remove duplicate code
	if (PrimaryAbilityState == EAbilityState::EAS_Cooldown)
	{
		PrimaryCooldownTime += DeltaTime;
		if (PrimaryCooldownTime >= PrimaryAbility->GetCooldownDuration())
		{
			PrimaryAbilityState = EAbilityState::EAS_Idle;
			PrimaryCooldownTime = 0.f;
		}
	}

	if (SecondaryAbilityState == EAbilityState::EAS_Cooldown)
	{
		SecondaryCooldownTime += DeltaTime;
		if (SecondaryCooldownTime >= SecondaryAbility->GetCooldownDuration())
		{
			SecondaryAbilityState = EAbilityState::EAS_Idle;
			SecondaryCooldownTime = 0.f;
		}
	}
}

void UAbilityComponent::DrawDebugInfo() const
{
	if (CVarDebugAbilityComponent->GetBool())
	{
		const FString LastAbilityName = (LastAbility != nullptr)? IAbility::Execute_GetName(LastAbility).ToString() : TEXT("None");
		const float LastAbilityDuration = (LastAbility != nullptr)? LastAbility->GetDuration() : 0.f;
		const FString DebugInfo = FString::Printf(TEXT("LastAbility: %s\n"
				"PrimaryState: %s | SecondaryState: %s\n"
				"Last Ability Duration: %.2f | Current Time: %.2f\n"
				"Primary Cooldown Duration: %.2f | Current Cooldown: %.2f\n"
				"Secondary Cooldown Duration: %.2f | Current Cooldown: %.2f\n"
			),
			*LastAbilityName,
			*UEnum::GetValueAsString(PrimaryAbilityState), *UEnum::GetValueAsString(SecondaryAbilityState),
			LastAbilityDuration, LastAbilityCurrentTime,
			PrimaryAbility->GetCooldownDuration(), PrimaryCooldownTime,
			SecondaryAbility->GetCooldownDuration(), SecondaryCooldownTime
		);
		
		const FVector OwnerLocation = GetOwner()->GetActorLocation();
		const FVector TextLocation = {OwnerLocation.X, OwnerLocation.Y, OwnerLocation.Z + 150.f};
		DrawDebugString(GetWorld(), TextLocation, DebugInfo, nullptr, FColor::Red, 0.0001f);
	}
}

bool UAbilityComponent::CanApplyPrimaryAbility() const
{
	if (PrimaryAbility == nullptr) { return false; }
	return PrimaryAbilityState == EAbilityState::EAS_Idle && SecondaryAbilityState != EAbilityState::EAS_Active;
}

bool UAbilityComponent::CanApplySecondaryAbility() const
{
	if (SecondaryAbility == nullptr) { return false; }
	return SecondaryAbilityState == EAbilityState::EAS_Idle && PrimaryAbilityState != EAbilityState::EAS_Active;
}

