// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RequestWalk.generated.h"

/**
 * 
 */
UCLASS()
class STELLARHARVEST_API UBTTask_RequestWalk : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RequestWalk();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
