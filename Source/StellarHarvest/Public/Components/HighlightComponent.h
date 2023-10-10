// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HighlightComponent.generated.h"

class UWidgetComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, ToolTip="Give player visual feedback enabling custom depth and custom widget on owner.") )
class STELLARHARVEST_API UHighlightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHighlightComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;

	void ActivateHighlightMesh() const;
	void DeactivateHighlightMesh() const;

	void ShowWidget() const;
	void HideWidget() const;

// Component interface
public:
	void ActivateHighlight() const;
	void DeactivateHighlight() const;

	UFUNCTION(BlueprintCallable)
	void SetMeshComponent(UPrimitiveComponent* NewMeshComponent);

	UFUNCTION(BlueprintCallable)
	void SetWidgetComponent(UWidgetComponent* NewWidgetComponent);

	UFUNCTION(BlueprintPure)
	FORCEINLINE UPrimitiveComponent* GetMeshComponent() const { return MeshComponent; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UWidgetComponent* GetWidgetComponent() const { return WidgetComponent; }
	
// Properties
protected:
	UPROPERTY(EditAnywhere, Category="Highlight")
	bool bShouldHighlightMesh = true;

	UPROPERTY(EditAnywhere, Category="Highlight")
	bool bShouldShowWidget = true;

	UPROPERTY(Transient)
	TObjectPtr<UPrimitiveComponent> MeshComponent;

	UPROPERTY(Transient)
	TObjectPtr<UWidgetComponent> WidgetComponent;

};
