// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_FindClosestPatrolPoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AIPatrolMovementComponent.h"

UBTTask_FindClosestPatrolPoint::UBTTask_FindClosestPatrolPoint()
{
	NodeName = TEXT("Find Closest Patrol Point");
}

EBTNodeResult::Type UBTTask_FindClosestPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (OwnerController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APawn* OwnerPawn = OwnerController->GetPawn();
	if (OwnerPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UAIPatrolMovementComponent* PatrolComponent = OwnerPawn->FindComponentByClass<UAIPatrolMovementComponent>();
	if (PatrolComponent == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(ClosestPointBlackboardKey.SelectedKeyName, PatrolComponent->GetNearestLocationOnPath());
	return EBTNodeResult::Succeeded;
}
