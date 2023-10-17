// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIMonsterController.generated.h"

class AStellarBaseCharacter;
class UAITargetComponent;
struct FAIStimulus;

/**
 * 
 */
UCLASS()
class STELLARHARVEST_API AAIMonsterController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	void StartSearchTargetLogic() const;
	
// Properties
protected:
	UPROPERTY(EditAnywhere, Category="Monster")
	TObjectPtr<UBehaviorTree> DefaultBehaviour;

	UPROPERTY(Transient)
	TObjectPtr<UAITargetComponent> TargetComponent;
	
	UPROPERTY(Transient)
	TObjectPtr<AStellarBaseCharacter> MonsterRef;
};
