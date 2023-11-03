// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AITargetComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AffiliationComponent.h"
#include "Components/AIPatrolMovementComponent.h"
#include "Utils/ActorUtils.h"

static TAutoConsoleVariable<bool> CVarDebugAITarget(
	TEXT("StellarHarvest.AI.TargetComponent"),
	false,
	TEXT("Show debug info targeting system for AI."),
	ECVF_Default
);

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
	SetupPerception();
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
	bDebugSawSomething = bIsTrackingTarget;
	OwnerController->GetBlackboardComponent()->SetValueAsBool(HasTargetBlackboardKey, bIsTrackingTarget);
	OwnerController->GetBlackboardComponent()->SetValueAsBool(IsInvestigatingBlackboardKey, !bIsTrackingTarget);
	OwnerController->GetBlackboardComponent()->SetValueAsObject(TargetObjectBlackboardKey, SourceActor);
	
	if (!bIsTrackingTarget)
	{
		OnLoseTarget();
	}
	else
	{
		State = ETargetingState::ETS_Tracking;
		RotateTowardsStimuli(SourceActor);
		TargetRef = SourceActor;
		OnTargetAcquired.Broadcast(TargetRef);
	}

	StopPatrolling();
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
	bDebugHeardSomething = bIsInvestigating;
	OwnerController->GetBlackboardComponent()->SetValueAsBool(IsInvestigatingBlackboardKey, bIsInvestigating);
	OwnerController->GetBlackboardComponent()->SetValueAsVector(InvestigationLocationBlackboardKey, Stimulus.StimulusLocation);

	if (bIsInvestigating)
	{
		StopPatrolling();
		State = ETargetingState::ETS_Investigating;
		CurrentTimeInvestigation = 0.f;
		OnInvestigationStart.Broadcast();
		RotateTowardsStimuli(SourceActor);
	}
	else
	{
		State = ETargetingState::ETS_Default;
	}
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

		if (!OwnerController->LineOfSightTo(TargetRef))
		{
			OnLoseTarget();
		}
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
			OnInvestigationStop.Broadcast();
		}
	}
}

void UAITargetComponent::StopPatrolling() const
{
	ensure(OwnerController != nullptr && OwnerController->GetPawn() != nullptr);

	UAIPatrolMovementComponent* PatrolMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UAIPatrolMovementComponent>();
	if (PatrolMovementComponent == nullptr)
	{
		return;
	}

	PatrolMovementComponent->Stop();
}

void UAITargetComponent::RotateTowardsStimuli(AActor* StimuliActor) const
{
	ensure(OwnerController != nullptr);
	
	if (bRotateTowardsStimuli)
	{
		OwnerController->SetFocus(StimuliActor, EAIFocusPriority::Gameplay);
	}
}

void UAITargetComponent::DrawDebug(const float DeltaTime)
{
	ensure(OwnerController != nullptr && OwnerController->GetPawn() != nullptr);
	
	if (CVarDebugAITarget->GetBool())
	{
		CurrentDebugTime += DeltaTime;
		if (CurrentDebugTime > 5.f)
		{
			CurrentDebugTime = 0.f;
			bDebugSawSomething = false;
			bDebugHeardSomething = false;
		}

		const FString TargetName = (TargetRef != nullptr)? TargetRef->GetActorLabel() : TEXT("None");
		const FString SawSomething		= (bDebugSawSomething)? TEXT("Yes") : TEXT("No");
		const FString HeardSomething	= (bDebugHeardSomething)? TEXT("Yes") : TEXT("No");
		
		const FString DebugString = FString::Printf(
			TEXT("<!-- AITarget -->\nState: %s | Target: %s | LastLocation: %s\n"
					"<!-- Investigation --> Total: %.2f | Current: %.2f\n"
					"<!-- Stimuli --> Saw: %s | Heard: %s"),
				*UEnum::GetValueAsString(State), *TargetName, *LastKnownLocation.ToString(),
				MaxTimeInvestigation, CurrentTimeInvestigation,
				*SawSomething, *HeardSomething
		);
		
		const FVector PawnLocation = OwnerController->GetPawn()->GetActorLocation();
		const FVector DebugLocation = {PawnLocation.X, PawnLocation.Y, PawnLocation.Z + 150.f};
		DrawDebugString(GetWorld(), DebugLocation, DebugString, nullptr, FColor::Black, 0.001f);
	}
}

void UAITargetComponent::OnLoseTarget()
{
	State = ETargetingState::ETS_Investigating;
	CurrentTimeInvestigation = 0.f;
	OwnerController->GetBlackboardComponent()->SetValueAsVector(InvestigationLocationBlackboardKey, LastKnownLocation);
	AActor* LastTarget = TargetRef;
	TargetRef = nullptr;
	OwnerController->ClearFocus(EAIFocusPriority::Gameplay);
	OnTargetLost.Broadcast(LastTarget);
	OnInvestigationStart.Broadcast();
}

void UAITargetComponent::ResetBlackboard() const
{
	ensure(OwnerController != nullptr && OwnerController->GetBlackboardComponent() != nullptr);

	OwnerController->GetBlackboardComponent()->SetValueAsBool(HasTargetBlackboardKey, false);
	OwnerController->GetBlackboardComponent()->SetValueAsObject(TargetObjectBlackboardKey, nullptr);
	OwnerController->GetBlackboardComponent()->SetValueAsBool(IsInvestigatingBlackboardKey, false);
}

void UAITargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TrackTarget();
	Investigate(DeltaTime);
	DrawDebug(DeltaTime);
}

void UAITargetComponent::StartSearch()
{
	bCanUsePerception = true;
}

void UAITargetComponent::StopSearch()
{
	ensure(OwnerController != nullptr);
	OwnerController->ClearFocus(EAIFocusPriority::Gameplay);
	ResetBlackboard();
	TargetRef = nullptr;
	State = ETargetingState::ETS_Default;
	bCanUsePerception = false;
}

void UAITargetComponent::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	ensure(OwnerController != nullptr);
	ensure(OwnerController->GetBlackboardComponent() != nullptr);

	if (!bCanUsePerception)
	{
		return;
	}
	
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

