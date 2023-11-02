// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CounterMeasureComponent.h"

#include "Components/SphereComponent.h"
#include "Components/StunHandlerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/ActorUtils.h"

UCounterMeasureComponent::UCounterMeasureComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCounterMeasureComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCounterMeasureComponent::OnRegister()
{
	Super::OnRegister();
	USphereComponent* Volume = GetOwner()->FindComponentByClass<USphereComponent>();
	SetTriggerVolume(Volume);
	CurrentAmount = TotalAmount;
}

void UCounterMeasureComponent::BindTriggerVolume()
{
	if (TriggerVolumeRef == nullptr)
	{
		return;
	}

	TriggerVolumeRef->OnComponentBeginOverlap.AddDynamic(this, &UCounterMeasureComponent::OnVolumeStartOverlap);
	TriggerVolumeRef->OnComponentEndOverlap.AddDynamic(this, &UCounterMeasureComponent::OnVolumeFinishOverlap);
}

void UCounterMeasureComponent::UnBindTriggerVolume()
{
	if (TriggerVolumeRef == nullptr)
	{
		return;
	}

	TriggerVolumeRef->OnComponentBeginOverlap.RemoveDynamic(this, &UCounterMeasureComponent::OnVolumeStartOverlap);
	TriggerVolumeRef->OnComponentEndOverlap.RemoveDynamic(this, &UCounterMeasureComponent::OnVolumeFinishOverlap);
}

void UCounterMeasureComponent::ApplyStun(AActor* Target) const
{
	if (Target == nullptr)
	{
		return;
	}
	
	if (!TriggerVolumeRef->IsOverlappingActor(Target))
	{
		return;
	}

	UStunHandlerComponent* StunComponent = Target->FindComponentByClass<UStunHandlerComponent>();
	if (StunComponent == nullptr)
	{
		return;
	}

	// No cumulative stun
	if (StunComponent->IsUnderEffect())
	{
		return;
	}

	const float ActualStunDuration = FMath::FRandRange(
		FMath::Max(0.0f, StunDuration - StunRandomDeviation), (StunDuration + StunRandomDeviation)
	);
		
	StunComponent->Apply(ActualStunDuration);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Target->GetActorLocation(), FRotator::ZeroRotator);
}

void UCounterMeasureComponent::ApplyStunToAll()
{
	for (int32 Index = 0; Index < Enemies.Num(); ++Index)
	{
		if (!Enemies.IsValidIndex(Index))
		{
			continue;
		}
		
		const TWeakObjectPtr<AActor> Enemy = Enemies[Index];
		if (!Enemy.IsValid())
		{
			continue;
		}

		ApplyStun(Enemy.Get());
	}
}

void UCounterMeasureComponent::UpdateActive(const float DeltaTime)
{
	if (State != ECounterMeasureState::ECMS_Active)
	{
		return;
	}

	CurrentDuration += DeltaTime;
	if (CurrentDuration > Duration)
	{
		CurrentDuration = 0.f;
		State = (HasCounterMeasures())? ECounterMeasureState::ECMS_Cooldown : ECounterMeasureState::ECMS_Idle;
		OnFinishDelegate.Broadcast();
	}
}

void UCounterMeasureComponent::UpdateCooldown(const float DeltaTime)
{
	if (State != ECounterMeasureState::ECMS_Cooldown)
	{
		return;
	}
	
	CooldownCurrentTime += DeltaTime;
	if (CooldownCurrentTime > CooldownDuration)
	{
		CooldownCurrentTime = 0.f;
		State = ECounterMeasureState::ECMS_Idle;
		OnCooldownFinishedDelegate.Broadcast();
	}
}

void UCounterMeasureComponent::UpdateGradualStun(const float DeltaTime)
{
	if (StunMode != EStunDealMode::ESDM_Gradual || State != ECounterMeasureState::ECMS_Active || TriggerVolumeRef == nullptr)
	{
		return;
	}

	const float TriggerRadius = TriggerVolumeRef->GetScaledSphereRadius();
	const float CurrentRadius = FMath::GetMappedRangeValueClamped(
		TRange<float>{0.f, Duration},
		TRange<float>{0.f, TriggerRadius},
		CurrentDuration
	);
	const float CurrentRadiusSquared = CurrentRadius * CurrentRadius;

	const FVector OwnerLocation = GetOwner()->GetActorLocation();
	for (int32 Index = 0; Index < Enemies.Num(); ++Index)
	{
		if (!Enemies.IsValidIndex(Index) || !Enemies[Index].IsValid())
		{
			continue;
		}

		const FVector EnemyLocation = Enemies[Index]->GetActorLocation();
		const float Dist = FVector::DistSquared(EnemyLocation, OwnerLocation);
		if (Dist <= CurrentRadiusSquared)
		{
			ApplyStun(Enemies[Index].Get());
		}
	}
}

void UCounterMeasureComponent::CheckForNotStunned()
{
	if (StunMode == EStunDealMode::ESDM_Instant && State == ECounterMeasureState::ECMS_Active)
	{
		ApplyStunToAll();
	}
}

void UCounterMeasureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateGradualStun(DeltaTime);
	UpdateActive(DeltaTime);
	UpdateCooldown(DeltaTime);
	CheckForNotStunned();
}

void UCounterMeasureComponent::UseCounterMeasure()
{
	if (CurrentAmount <= 0 || State != ECounterMeasureState::ECMS_Idle)
	{
		return;
	}

	State = ECounterMeasureState::ECMS_Active;
	CurrentAmount = FMath::Clamp(CurrentAmount - 1, 0, TotalAmount);
	FireCounterMeasureLocation = GetOwner()->GetActorLocation();
	OnUseCounterMeasureDelegate.Broadcast(CurrentAmount);
	
	if (StunMode == EStunDealMode::ESDM_Instant)
	{
		ApplyStunToAll();
	}
}

void UCounterMeasureComponent::SetTriggerVolume(USphereComponent* Volume)
{
	if (Volume == nullptr || Volume == TriggerVolumeRef)
	{
		return;
	}

	if (Volume->GetOwner() != GetOwner())
	{
		return;
	}

	UnBindTriggerVolume();
	TriggerVolumeRef = Volume;
	BindTriggerVolume();
}

void UCounterMeasureComponent::OnVolumeStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
	{
		return;
	}

	if (UActorUtils::IsFriendly(GetOwner(), OtherActor))
	{
		return;
	}
	
	Enemies.AddUnique(OtherActor);
	if (State == ECounterMeasureState::ECMS_Active && StunMode == EStunDealMode::ESDM_Instant)
	{
		ApplyStun(OtherActor);
	}

	if (State == ECounterMeasureState::ECMS_Idle)
	{
		OnEnemiesInRangeDelegate.Broadcast();
	}
}

void UCounterMeasureComponent::OnVolumeFinishOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Enemies.Remove(OtherActor);
	
	if (Enemies.Num() <= 0)
	{
		OnLoseEnemiesInRangeDelegate.Broadcast();
	}
}

