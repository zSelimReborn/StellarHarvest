// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RequestAttack.generated.h"

struct FBlackboardKeySelector;

/**
 * 
 */
UCLASS()
class STELLARHARVEST_API UBTTask_RequestAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RequestAttack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
// Properties
protected:
	UPROPERTY(EditAnywhere, Category=Attack)
	FBlackboardKeySelector TargetObjectBlackboardKey;
};
