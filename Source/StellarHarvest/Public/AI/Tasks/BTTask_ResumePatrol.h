// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ResumePatrol.generated.h"

/**
 * 
 */
UCLASS()
class STELLARHARVEST_API UBTTask_ResumePatrol : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ResumePatrol();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
