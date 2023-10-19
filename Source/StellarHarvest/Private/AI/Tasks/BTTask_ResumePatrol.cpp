// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_ResumePatrol.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AIPatrolMovementComponent.h"

UBTTask_ResumePatrol::UBTTask_ResumePatrol()
{
	NodeName = TEXT("Resume Patrol");
}

EBTNodeResult::Type UBTTask_ResumePatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	PatrolComponent->Resume();
	return EBTNodeResult::Succeeded;
}
