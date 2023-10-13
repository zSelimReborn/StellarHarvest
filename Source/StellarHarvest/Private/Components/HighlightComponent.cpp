// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HighlightComponent.h"

#include "Components/WidgetComponent.h"

// Sets default values for this component's properties
UHighlightComponent::UHighlightComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UHighlightComponent::BeginPlay()
{
	Super::BeginPlay();
	DeactivateHighlight();
}

void UHighlightComponent::OnRegister()
{
	Super::OnRegister();

	if (GetOwner())
	{
		SetMeshComponent(GetOwner()->FindComponentByClass<UPrimitiveComponent>());
		SetWidgetComponent(GetOwner()->FindComponentByClass<UWidgetComponent>());
	}
}

void UHighlightComponent::ActivateHighlightMesh() const
{
	if (MeshComponent != nullptr)
	{
		MeshComponent->SetRenderCustomDepth(true);
	}
}

void UHighlightComponent::DeactivateHighlightMesh() const
{
	if (MeshComponent != nullptr)
	{
		MeshComponent->SetRenderCustomDepth(false);	
	}
}

void UHighlightComponent::ShowWidget() const
{
	if (WidgetComponent != nullptr)
	{
		WidgetComponent->SetVisibility(true);
	}
}

void UHighlightComponent::HideWidget() const
{
	if (WidgetComponent != nullptr)
	{
		WidgetComponent->SetVisibility(false);
	}
}

void UHighlightComponent::ActivateHighlight() const
{
	if (bShouldHighlightMesh)
	{
		ActivateHighlightMesh();
	}

	if (bShouldShowWidget)
	{
		ShowWidget();
	}

	OnActivateHighlightDelegate.Broadcast();
}

void UHighlightComponent::DeactivateHighlight() const
{
	if (bShouldHighlightMesh)
	{
		DeactivateHighlightMesh();
	}

	if (bShouldShowWidget)
	{
		HideWidget();
	}

	OnDeactivateHighlightDelegate.Broadcast();
}

void UHighlightComponent::SetMeshComponent(UPrimitiveComponent* NewMeshComponent)
{
	if (!NewMeshComponent)
	{
		return;
	}

	if (NewMeshComponent == MeshComponent)
	{
		return;
	}

	if (NewMeshComponent->GetOwner() != GetOwner())
	{
		return;
	}

	DeactivateHighlight();
	MeshComponent = NewMeshComponent;
}

void UHighlightComponent::SetWidgetComponent(UWidgetComponent* NewWidgetComponent)
{
	if (!NewWidgetComponent)
	{
		return;
	}

	if (NewWidgetComponent == WidgetComponent)
	{
		return;
	}

	if (NewWidgetComponent->GetOwner() != GetOwner())
	{
		return;
	}

	DeactivateHighlight();
	WidgetComponent = NewWidgetComponent;
}
