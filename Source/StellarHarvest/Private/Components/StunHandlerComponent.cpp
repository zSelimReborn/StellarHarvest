// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StunHandlerComponent.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/PawnMovementComponent.h"

static TAutoConsoleVariable<bool> CVarDebugStunStatus(
	TEXT("StellarHarvest.DebugStunStatus"),
	false,
	TEXT("Show debug info about stun status."),
	ECVF_Default
);

UStunHandlerComponent::UStunHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStunHandlerComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}

void UStunHandlerComponent::OnRegister()
{
	Super::OnRegister();
	OwnerPawn = Cast<APawn>(GetOwner());
	Duration = BaseDuration;
}

void UStunHandlerComponent::UpdateStatus(const float DeltaTime)
{
	if (!bIsUnderEffect)
	{
		SetComponentTickEnabled(false);
		return;
	}
	
	CurrentTime += DeltaTime;
	if (CurrentTime >= Duration)
	{
		OnStatusFinish();
		CurrentTime = 0.f;
		Duration = BaseDuration;
		SetComponentTickEnabled(false);
	}
}

void UStunHandlerComponent::OnStatusFinish()
{
	RemoveStun();
	OnStatusRemoved.Broadcast();
}

void UStunHandlerComponent::ApplyStun()
{
	ensure(OwnerPawn != nullptr);

	OwnerPawn->GetMovementComponent()->Deactivate();
	AAIController* Controller = OwnerPawn->GetController<AAIController>();
	if (Controller)
	{
		UBehaviorTreeComponent* BTComponent = Controller->FindComponentByClass<UBehaviorTreeComponent>();
		if (BTComponent)
		{
			BTComponent->PauseLogic(TEXT("Stunned"));
		}
	}
}

void UStunHandlerComponent::RemoveStun()
{
	ensure(OwnerPawn != nullptr);

	OwnerPawn->GetMovementComponent()->Activate();
	AAIController* Controller = OwnerPawn->GetController<AAIController>();
	if (Controller)
	{
		UBehaviorTreeComponent* BTComponent = Controller->FindComponentByClass<UBehaviorTreeComponent>();
		if (BTComponent)
		{
			BTComponent->ResumeLogic(TEXT("Not Stunned"));
		}
	}
}

void UStunHandlerComponent::DrawDebug() const
{
	if (CVarDebugStunStatus->GetBool())
	{
		FString DebugInfo = FString::Printf(TEXT("<!---STUN --->\nDuration: %.2f | Current: %.2f"), Duration, CurrentTime);
		const FVector OwnerLocation = GetOwner()->GetActorLocation();
		const FVector DebugLocation = {OwnerLocation.X, OwnerLocation.Y - 100.f, OwnerLocation.Z + 150.f};
		DrawDebugString(GetWorld(), DebugLocation, DebugInfo, nullptr, FColor::Black, 0.001);
	}
}

void UStunHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateStatus(DeltaTime);
	DrawDebug();
}

void UStunHandlerComponent::Apply(const float DeltaDuration)
{
	Duration += DeltaDuration;
	bIsUnderEffect = true;
	SetComponentTickEnabled(true);
	ApplyStun();
	OnStatusApplied.Broadcast(Duration);
}

