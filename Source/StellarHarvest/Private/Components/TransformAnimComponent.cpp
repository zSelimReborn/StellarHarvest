// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TransformAnimComponent.h"

UTransformAnimComponent::UTransformAnimComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UTransformAnimComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}

void UTransformAnimComponent::OnRegister()
{
	Super::OnRegister();
	USceneComponent* RootComponent = GetOwner()->GetRootComponent();
	SetUpdatedComponent(RootComponent);
}

void UTransformAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TickAnimation(DeltaTime);
}

void UTransformAnimComponent::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
	if (NewUpdatedComponent == nullptr)
	{
		return;
	}
	
	if (NewUpdatedComponent == UpdatedComponent)
	{
		return;
	}

	if (NewUpdatedComponent->GetOwner() != GetOwner())
	{
		return;
	}

	// TODO Maybe should finish animation on old component
	UpdatedComponent = NewUpdatedComponent;
	InitialLocation = UpdatedComponent->GetRelativeLocation();
	InitialRotation = UpdatedComponent->GetRelativeRotation();
}

void UTransformAnimComponent::StartAnimation()
{
	AnimState = ETractorAnimState::ETAS_Playing;
	SetComponentTickEnabled(true);
}

void UTransformAnimComponent::StopAnimation()
{
	AnimState = ETractorAnimState::ETAS_Stop;
	SetComponentTickEnabled(true);
}

void UTransformAnimComponent::ReverseAnimation()
{
	AnimState = ETractorAnimState::ETAS_Reverse;
	SetComponentTickEnabled(true);
}

void UTransformAnimComponent::TickAnimation(const float DeltaTime)
{
	if (!bAnimateLocation && !bAnimateRotation)
	{
		OnStopAnimation();
		return;
	}
	
	if (UpdatedComponent == nullptr)
	{
		OnStopAnimation();
		return;
	}

	const float Step = IsGoingBackward()? -DeltaTime : DeltaTime;
	CurrentAnimationTime = FMath::Clamp(CurrentAnimationTime + Step, 0.f, AnimationTime);
	
	const float TimeRatio = FMath::Clamp(CurrentAnimationTime / AnimationTime, 0.f, 1.f);
	const FVector NewLocation = FMath::Lerp(InitialLocation, TargetLocation, TimeRatio);
	const FRotator NewRotation = FMath::Lerp(InitialRotation, TargetRotation, TimeRatio);

	if (bAnimateLocation)
	{
		UpdatedComponent->SetRelativeLocation(NewLocation);
	}

	if (bAnimateRotation)
	{
		UpdatedComponent->SetRelativeRotation(NewRotation);
	}

	if (AnimState == ETractorAnimState::ETAS_Stop && TimeRatio <= 0.f)
	{
		OnStopAnimation();
		return;
	}

	bool bShouldStopAnimation = false;
	if (AnimState == ETractorAnimState::ETAS_Playing && TimeRatio >= 1.f)
	{
		AnimState = ETractorAnimState::ETAS_Reverse;
		bShouldStopAnimation = true;
	}
	else if (AnimState == ETractorAnimState::ETAS_Reverse && TimeRatio <= 0.f)
	{
		AnimState = ETractorAnimState::ETAS_Playing;
		bShouldStopAnimation = true;
	}

	if (bShouldStopAnimation && !bLoopAnimation)
	{
		OnStopAnimation();
		return;
	}
}

void UTransformAnimComponent::OnStopAnimation()
{
	AnimState = ETractorAnimState::ETAS_Default;
	SetComponentTickEnabled(false);
}

