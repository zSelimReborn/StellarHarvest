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
	if (AffiliationComponent == nullptr)
	{
		return false;
	}
	
	return AffiliationComponent->IsFriendly(TestingActor);
}

void UAITargetComponent::OnSightStimulus(AActor* SourceActor, const FAIStimulus& Stimulus)
{
	ensure(OwnerController != nullptr);
	
	const bool bIsTrackingTarget = Stimulus.WasSuccessfullySensed();
	OwnerController->GetBlackboardComponent()->SetValueAsBool(HasTargetBlackboardKey, bIsTrackingTarget);
	OwnerController->GetBlackboardComponent()->SetValueAsBool(IsInvestigatingBlackboardKey, !bIsTrackingTarget);
	OwnerController->GetBlackboardComponent()->SetValueAsObject(TargetObjectBlackboardKey, SourceActor);
	TargetRef = SourceActor;
	State = (bIsTrackingTarget)? ETargetingState::ETS_Tracking : ETargetingState::ETS_Investigating;
	
	if (!bIsTrackingTarget)
	{
		CurrentTimeInvestigation = 0.f;
		OwnerController->GetBlackboardComponent()->SetValueAsVector(InvestigationLocationBlackboardKey, LastKnownLocation);
	}
}

void UAITargetComponent::OnHearStimulus(AActor* SourceActor, const FAIStimulus& Stimulus)
{
	ensure(OwnerController != nullptr);

	// If we currently have a target, skip investigation
	if (TargetRef != nullptr)
	{
		return;
	}
	
	const bool bIsInvestigating = Stimulus.WasSuccessfullySensed();
	OwnerController->GetBlackboardComponent()->SetValueAsBool(IsInvestigatingBlackboardKey, bIsInvestigating);
	OwnerController->GetBlackboardComponent()->SetValueAsVector(InvestigationLocationBlackboardKey, Stimulus.StimulusLocation);
	State = (bIsInvestigating)? ETargetingState::ETS_Investigating : ETargetingState::ETS_Default;
	CurrentTimeInvestigation = (bIsInvestigating)? 0.f : CurrentTimeInvestigation;
}

void UAITargetComponent::TrackTarget()
{
	ensure(OwnerController != nullptr);
	
	if (State == ETargetingState::ETS_Tracking && TargetRef != nullptr)
	{
		const float DistanceToTarget = FVector::Dist(OwnerController->GetPawn()->GetActorLocation(), TargetRef->GetActorLocation());
		OwnerController->GetBlackboardComponent()->SetValueAsVector(TargetLocationBlackboardKey, TargetRef->GetActorLocation());
		OwnerController->GetBlackboardComponent()->SetValueAsFloat(TargetDistanceBlackboardKey, DistanceToTarget);
		LastKnownLocation = TargetRef->GetActorLocation();
	}
}

void UAITargetComponent::Investigate(const float DeltaTime)
{
	ensure(OwnerController != nullptr);

	if (State == ETargetingState::ETS_Investigating)
	{
		// Extra for safety
		CurrentTimeInvestigation = FMath::Clamp(CurrentTimeInvestigation + DeltaTime, 0.f, MaxTimeInvestigation);
		const FVector PawnLocation = OwnerController->GetPawn()->GetActorLocation();
		if (FVector::DistSquaredXY(PawnLocation, LastKnownLocation) <= 100.f || CurrentTimeInvestigation >= MaxTimeInvestigation)
		{
			OwnerController->GetBlackboardComponent()->SetValueAsBool(IsInvestigatingBlackboardKey, false);
			State = ETargetingState::ETS_Default;
			CurrentTimeInvestigation = 0.f;
		}
	}
}

void UAITargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TrackTarget();
	Investigate(DeltaTime);
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

