// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/AIMonsterController.h"

#include "Characters/StellarBaseCharacter.h"
#include "Components/AITargetComponent.h"

void AAIMonsterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	MonsterRef = Cast<AStellarBaseCharacter>(InPawn);
	TargetComponent = FindComponentByClass<UAITargetComponent>();
	if (DefaultBehaviour != nullptr)
	{
		RunBehaviorTree(DefaultBehaviour);
	}

	StartSearchTargetLogic();
}

void AAIMonsterController::OnUnPossess()
{
	Super::OnUnPossess();
	if (TargetComponent != nullptr)
	{
		TargetComponent->StopSearch();
	}
}

void AAIMonsterController::StartSearchTargetLogic() const
{
	if (TargetComponent != nullptr)
	{
		TargetComponent->StartSearch();
	}
}
