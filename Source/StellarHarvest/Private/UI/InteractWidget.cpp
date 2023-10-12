// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractWidget.h"

#include "Components/TextBlock.h"

FString UInteractWidget::GetInteractText() const
{
	return InteractText->GetText().ToString();
}

void UInteractWidget::SetInteractText(const FString& NewInteractText)
{
	if (!NewInteractText.IsEmpty())
	{
		InteractText->SetText(FText::FromString(NewInteractText));
	}
	else
	{
		ResetInteractText();
	}
}

FString UInteractWidget::GetDefaultInteractText() const
{
	return TEXT("Interact");
}

void UInteractWidget::ResetInteractText()
{
	InteractText->SetText(FText::FromString(GetDefaultInteractText()));
}
