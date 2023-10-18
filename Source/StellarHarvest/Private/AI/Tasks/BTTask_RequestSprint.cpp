// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_RequestSprint.h"

#include "AIController.h"
#include "Characters/StellarBaseCharacter.h"

UBTTask_RequestSprint::UBTTask_RequestSprint()
{
	NodeName = TEXT("Request Sprint");
}

EBTNodeResult::Type UBTTask_RequestSprint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (OwnerController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	const AStellarBaseCharacter* Character = Cast<AStellarBaseCharacter>(OwnerController->GetCharacter());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Character->RequestSprint();
	return EBTNodeResult::Succeeded;
}
