// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/ActorUtils.h"

#include "Components/AffiliationComponent.h"

bool UActorUtils::IsVisibleOnScreen(const APlayerController* PC, const AActor* TestingActor)
{
	if (PC == nullptr || TestingActor == nullptr)
	{
		return false;
	}

	int32 ViewportX{}, ViewportY{};
	PC->GetViewportSize(ViewportX, ViewportY);

	FVector2D ActorLocationOnScreen{};
	if (PC->ProjectWorldLocationToScreen(TestingActor->GetActorLocation(), ActorLocationOnScreen))
	{
		return ActorLocationOnScreen.X >= 0.f &&
				ActorLocationOnScreen.X <= ViewportX &&
					ActorLocationOnScreen.Y >= 0.f &&
						ActorLocationOnScreen.Y <= ViewportY;
	}
	else
	{
		return false;
	}
}

void UActorUtils::RotateTowardsTarget(AActor* Source, const AActor* TargetActor, const bool bOverrideZ)
{
	if (Source == nullptr || TargetActor == nullptr)
	{
		return;
	}

	const FVector SourceLocation = Source->GetActorLocation();
	const FVector TargetLocation = TargetActor->GetActorLocation();
	FVector Direction = TargetLocation - SourceLocation;
	if (!bOverrideZ)
	{
		Direction.Z = SourceLocation.Z;
	}
	
	Source->SetActorRotation(Direction.Rotation());
}

bool UActorUtils::IsFriendly(const AActor* Source, const AActor* Target)
{
	if (Source == nullptr || Target == nullptr)
	{
		return true;
	}

	if (Source == Target)
	{
		return true;
	}

	const UAffiliationComponent* SourceAffiliation = Source->FindComponentByClass<UAffiliationComponent>();
	const UAffiliationComponent* TargetAffiliation = Target->FindComponentByClass<UAffiliationComponent>();
	if (SourceAffiliation == nullptr || TargetAffiliation == nullptr)
	{
		return true;
	}
	
	return SourceAffiliation->GetAffiliation().IsEqual(TargetAffiliation->GetAffiliation());
}
