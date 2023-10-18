// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_RequestAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/DamageEvents.h"

UBTTask_RequestAttack::UBTTask_RequestAttack()
{
	NodeName = TEXT("Request Attack");
}

EBTNodeResult::Type UBTTask_RequestAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (OwnerController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetObjectBlackboardKey.SelectedKeyName);
	if (TargetObject == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(TargetObject);
	if (TargetActor == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	const FDamageEvent DamageEvent{UDamageType::StaticClass()};
	TargetActor->TakeDamage(1.f, DamageEvent, OwnerController, OwnerController->GetPawn());
	return EBTNodeResult::Succeeded;
}
