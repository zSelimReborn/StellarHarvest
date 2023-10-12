// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

class UTextBlock;

UCLASS()
class STELLARHARVEST_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FString GetInteractText() const;

	UFUNCTION(BlueprintCallable)
	void SetInteractText(const FString& NewInteractText);

	UFUNCTION(BlueprintCallable)
	FString GetDefaultInteractText() const;

	UFUNCTION(BlueprintCallable)
	void ResetInteractText();
	
	// Mandatory widgets
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> InteractText;

};
