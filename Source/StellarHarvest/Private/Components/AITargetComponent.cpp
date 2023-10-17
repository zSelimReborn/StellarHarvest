// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AITargetComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AffiliationComponent.h"

UAITargetComponent::UAITargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAITargetComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UAITargetComponent::OnRegister()
{
	Super::OnRegister();
	OwnerController = Cast<AAIController>(GetOwner());
}

void UAITargetComponent::SetupPerception()
{
	OwnerController = Cast<AAIController>(GetOwner());
	ensure(OwnerController != nullptr);
	
	UAIPerceptionComponent* PerceptionComponent = OwnerController->FindComponentByClass<UAIPerceptionComponent>();
	if (PerceptionComponent != nullptr)
	{
		if (!PerceptionComponent->OnTargetPerceptionUpdated.IsAlreadyBound(this, &UAITargetComponent::OnPerception))
		{
			PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &UAITargetComponent::OnPerception);
		}
	}
}

bool UAITargetComponent::IsFriendly(const AActor* TestingActor) const
{
	ensure(OwnerController != nullptr);

	const UAffiliationComponent* AffiliationComponent = OwnerController->GetPawn()->FindComponentByClass<UAffiliationComponent>();
	return AffiliationComponent->IsFriendly(TestingActor);
}

void UAITargetComponent::OnSightStimulus(AActor* SourceActor, const FAIStimulus& Stimulus)
{
	ensure(OwnerController != nullptr);
	
	bIsTrackingTarget = Stimulus.WasSuccessfullySensed();
	OwnerController->GetBlackboardComponent()->SetValueAsBool(HasTargetBlackboardKey, bIsTrackingTarget);

	if (!bIsTrackingTarget)
	{
		OwnerController->GetBlackboardComponent()->SetValueAsVector(LastKnownTargetLocationBlackboardKey, SourceActor->GetActorLocation());
	}
}

void UAITargetComponent::OnHearStimulus(AActor* SourceActor, const FAIStimulus& Stimulus)
{
}

void UAITargetComponent::TrackTarget() const
{
	ensure(OwnerController != nullptr);
	
	if (bIsTrackingTarget && TargetRef != nullptr)
	{
		const float DistanceToTarget = FVector::Dist(OwnerController->GetPawn()->GetActorLocation(), TargetRef->GetActorLocation());
		OwnerController->GetBlackboardComponent()->SetValueAsVector(TargetLocationBlackboardKey, TargetRef->GetActorLocation());
		OwnerController->GetBlackboardComponent()->SetValueAsFloat(TargetDistanceBlackboardKey, DistanceToTarget);
	}
}

void UAITargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TrackTarget();
}

void UAITargetComponent::StartSearch()
{
	SetupPerception();
}

void UAITargetComponent::StopSearch()
{
	
}

void UAITargetComponent::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	ensure(OwnerController != nullptr);
	ensure(OwnerController->GetBlackboardComponent() != nullptr);
	
	// Skip other monsters
	if (IsFriendly(Actor))
	{
		return;
	}

	// Sight stimulus
	if (Stimulus.Type == UAISense::GetSenseID(SightSense))
	{
		OnSightStimulus(Actor, Stimulus);
	}
	// Hear stimulus
	else if (Stimulus.Type == UAISense::GetSenseID(HearSense))
	{
		OnHearStimulus(Actor, Stimulus);
	}
}

