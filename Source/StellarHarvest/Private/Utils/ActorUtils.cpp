// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/ActorUtils.h"

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
