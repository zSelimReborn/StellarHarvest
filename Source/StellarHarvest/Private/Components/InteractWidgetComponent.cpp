// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractWidgetComponent.h"

#include "UI/InteractWidget.h"

void UInteractWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupInteractText();
}

void UInteractWidgetComponent::SetupInteractText() const
{
	if (GetWidget() != nullptr)
	{
		UInteractWidget* InteractWidget = Cast<UInteractWidget>(GetWidget());
		if (InteractWidget != nullptr)
		{
			InteractWidget->SetInteractText(InteractText);
		}
	}
}
