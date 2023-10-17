// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AffiliationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STELLARHARVEST_API UAffiliationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAffiliationComponent();

protected:
	virtual void BeginPlay() override;
	
	virtual void OnRegister() override;

	virtual void OnUnregister() override;
	
public:
	UFUNCTION(BlueprintPure)
	bool IsFriendly(const AActor* TestingActor) const;

	UFUNCTION(BlueprintCallable)
	void ChangeAffiliation(const FName& NewAffiliation);

	UFUNCTION(BlueprintCallable)
	FName GetAffiliation() const;

	UFUNCTION(BlueprintCallable)
	void RemoveAffiliation();

// Properties
protected:
	UPROPERTY(EditAnywhere, Category="Affiliation")
	FName AffiliationTag = NAME_None;
};
