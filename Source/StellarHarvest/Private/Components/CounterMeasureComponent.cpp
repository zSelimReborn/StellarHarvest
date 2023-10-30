// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CounterMeasureComponent.h"

#include "Components/ShapeComponent.h"

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

void UCounterMeasureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCounterMeasureComponent::UseCounterMeasure()
{
	if (CurrentAmount <= 0)
	{
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("Use Counter Measure"));
	CurrentAmount = FMath::Clamp(CurrentAmount - 1, 0, TotalAmount);
	OnUseCounterMeasureDelegate.Broadcast(CurrentAmount);
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

	if (!OtherActor->IsA(EnemyClass))
	{
		return;
	}

	Enemies.Add(OtherActor);
	OnEnemiesInRangeDelegate.Broadcast();
}

void UCounterMeasureComponent::OnVolumeFinishOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr)
	{
		return;
	}

	if (!OtherActor->IsA(EnemyClass))
	{
		return;
	}

	Enemies.Remove(OtherActor);
	
	if (Enemies.Num() <= 0)
	{
		OnLoseEnemiesInRangeDelegate.Broadcast();
	}
}

