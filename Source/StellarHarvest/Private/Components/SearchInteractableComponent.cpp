// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SearchInteractableComponent.h"

#include "Components/HighlightComponent.h"
#include "Interfaces/Interactable.h"

// Sets default values for this component's properties
USearchInteractableComponent::USearchInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USearchInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(bShouldSearchForItems);
}

void USearchInteractableComponent::SearchForInteractables()
{
	const FVector OwnerLocation = GetOwner()->GetActorLocation();
	const FVector OwnerForward = GetOwner()->GetActorForwardVector();

	const FVector EndTrace = OwnerLocation + OwnerForward * SearchTraceLength;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(SphereSearchRadius);

	FCollisionQueryParams QueryParams{TEXT("StellarHarvest::SearchForItems")};
	QueryParams.AddIgnoredActor(GetOwner());
		
	FHitResult TraceResult;
	const bool bHitSomething = GetWorld()->SweepSingleByChannel(
		TraceResult,
		OwnerLocation,
		EndTrace,
		FQuat::Identity,
		SearchChannel,
		Sphere,
		QueryParams
	);

	AActor* FoundItem = (TraceResult.GetActor());
	if (bHitSomething && FoundItem && FoundItem->Implements<UInteractable>())
	{
		if (FoundItem != ItemFoundRef)
		{
			NewItemFoundDelegate.Broadcast(TraceResult, FoundItem);
			ItemFoundRef = FoundItem;
			ToggleHighlightItem(ItemFoundRef.Get(), true);
		}
	}
	else if (ItemFoundRef != nullptr)
	{
		ItemLostDelegate.Broadcast(ItemFoundRef.Get());
		ToggleHighlightItem(ItemFoundRef.Get(), false);
		ItemFoundRef = nullptr;
	}
}

void USearchInteractableComponent::ToggleHighlightItem(const AActor* HighlightActor, const bool bHighlight) const
{
	if (HighlightActor)
	{
		const UHighlightComponent* HighlightComponent = HighlightActor->FindComponentByClass<UHighlightComponent>();
		if (HighlightComponent)
		{
			if (bHighlight)
			{
				HighlightComponent->ActivateHighlight();
			}
			else
			{
				HighlightComponent->DeactivateHighlight();
			}
		}
	}
}


// Called every frame
void USearchInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SearchForInteractables();
}

AActor* USearchInteractableComponent::GetFoundItem() const
{
	return ItemFoundRef.Get();
}

