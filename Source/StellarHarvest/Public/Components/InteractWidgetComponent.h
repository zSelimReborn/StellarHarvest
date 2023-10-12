// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractWidgetComponent.generated.h"

class UInteractWidget;

/**
 * 
 */
UCLASS( ClassGroup=("UserInterface"), meta=(BlueprintSpawnableComponent, ToolTip="Allows to spawn an InteractWidget and customize it.") )
class STELLARHARVEST_API UInteractWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void SetupInteractText() const;
	
// Properties
protected:
	UPROPERTY(EditAnywhere, Category="Interact")
	FString InteractText;
};
