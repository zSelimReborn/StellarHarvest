// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SearchInteractableComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNewItemFoundDelegate, const FHitResult&, HitResult, AActor*, NewItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemLostDelegate, AActor*, LostItem);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, ToolTip="Search for interactable items in front of the current character/actor."))
class STELLARHARVEST_API USearchInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USearchInteractableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	void SearchForInteractables();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Events
public:
	FOnNewItemFoundDelegate& OnNewItemFound() { return NewItemFoundDelegate; }
	FOnItemLostDelegate& OnItemLost() { return ItemLostDelegate; }

// Properties
protected:
	UPROPERTY(EditAnywhere, Category="Search")
	bool bShouldSearchForItems = true;
	
	UPROPERTY(EditAnywhere, Category="Search")
	float SearchTraceLength = 1000.f;

	UPROPERTY(EditAnywhere, Category="Search")
	float SphereSearchRadius = 50.f;

	UPROPERTY(EditAnywhere, Category="Search")
	TEnumAsByte<ECollisionChannel> SearchChannel = ECC_WorldDynamic;

	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> ItemFoundRef;

// Delegates
protected:
	FOnNewItemFoundDelegate NewItemFoundDelegate;
	FOnItemLostDelegate ItemLostDelegate;
	
};
