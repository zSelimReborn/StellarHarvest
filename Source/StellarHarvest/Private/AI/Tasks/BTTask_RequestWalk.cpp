// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_RequestWalk.h"
#include "AIController.h"
#include "Characters/StellarBaseCharacter.h"

UBTTask_RequestWalk::UBTTask_RequestWalk()
{
	NodeName = TEXT("Request Walk");
}

EBTNodeResult::Type UBTTask_RequestWalk::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	Character->RequestWalk();
	return EBTNodeResult::Succeeded;

}
