// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CounterMeasureComponent.h"

#include "Components/ShapeComponent.h"
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
	UShapeComponent* Volume = GetOwner()->FindComponentByClass<UShapeComponent>();
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

void UCounterMeasureComponent::ApplyStun()
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

		if (!TriggerVolumeRef->IsOverlappingActor(Enemy.Get()))
		{
			continue;
		}

		UStunHandlerComponent* StunComponent = Enemy->FindComponentByClass<UStunHandlerComponent>();
		if (StunComponent == nullptr)
		{
			continue;
		}

		const float StunDuration = FMath::FRandRange(
			FMath::Max(0.0f, Duration - RandomDeviation), (Duration + RandomDeviation)
		);
		
		StunComponent->Apply(StunDuration);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), StunParticle, Enemy->GetActorLocation(), FRotator::ZeroRotator);
	}
}

void UCounterMeasureComponent::UpdateCooldown(const float DeltaTime)
{
	if (bIsUsable)
	{
		return;
	}
	
	CooldownCurrentTime += DeltaTime;
	if (CooldownCurrentTime > CooldownDuration)
	{
		CooldownCurrentTime = 0.f;
		bIsUsable = true;
		OnCooldownFinishedDelegate.Broadcast();
	}
}

void UCounterMeasureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateCooldown(DeltaTime);
}

void UCounterMeasureComponent::UseCounterMeasure()
{
	if (CurrentAmount <= 0 || !bIsUsable)
	{
		return;
	}

	CurrentAmount = FMath::Clamp(CurrentAmount - 1, 0, TotalAmount);
	OnUseCounterMeasureDelegate.Broadcast(CurrentAmount);
	bIsUsable = false;
	ApplyStun();
}

void UCounterMeasureComponent::SetTriggerVolume(UShapeComponent* Volume)
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
	OnEnemiesInRangeDelegate.Broadcast();
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

