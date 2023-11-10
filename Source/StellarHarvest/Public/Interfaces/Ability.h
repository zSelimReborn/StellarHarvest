// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Ability.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAbility : public UInterface
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FAbilityData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Texture;
};

/**
 * 
 */
class STELLARHARVEST_API IAbility
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category="Ability")
	void Apply(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category="Ability")
	void Remove(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Ability")
	FName GetAbilityId() const;
};
