// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AbilityComponent.h"

#include "Gameplay/Abilities/EffectAbility.h"
#include "Interfaces/Ability.h"

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
}

void UAbilityComponent::ApplyPrimaryAbility()
{
	if (!CanApplyAbility() || PrimaryAbility == nullptr)
	{
		return;
	}

	ApplyAbility(PrimaryAbility);
	PrimaryAbilityState = EAbilityState::EAS_Active;
}

void UAbilityComponent::ApplySecondaryAbility()
{
	if (!CanApplyAbility() || SecondaryAbility == nullptr)
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
	if (PrimaryAbilityState != EAbilityState::EAS_Active || SecondaryAbilityState != EAbilityState::EAS_Active)
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
	if (PrimaryAbilityState != EAbilityState::EAS_Cooldown || SecondaryAbilityState != EAbilityState::EAS_Cooldown)
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

bool UAbilityComponent::CanApplyAbility() const
{
	// TODO check
	return PrimaryAbilityState != EAbilityState::EAS_Active &&
		SecondaryAbilityState != EAbilityState::EAS_Active;
}

