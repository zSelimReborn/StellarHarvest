// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindClosestPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class STELLARHARVEST_API UBTTask_FindClosestPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindClosestPatrolPoint();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
// Properties
protected:
	UPROPERTY(EditAnywhere, Category=Patrol)
	FBlackboardKeySelector ClosestPointBlackboardKey;

};
