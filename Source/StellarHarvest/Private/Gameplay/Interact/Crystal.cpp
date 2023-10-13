// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Interact/Crystal.h"

#include "Components/CrystalCollectorComponent.h"
#include "Components/BoxComponent.h"
#include "Components/HighlightComponent.h"
#include "Components/InteractWidgetComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/CrystalClusterWidget.h"

static TAutoConsoleVariable<bool> CVarDebugCrystalCluster(
	TEXT("StellarHarvest.Crystal.DebugCrystalCluster"),
	false,
	TEXT("Show debug info crystal cluster status."),
	ECVF_Default
);

// Sets default values
ACrystal::ACrystal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Component"));
	SetRootComponent(DefaultSceneComponent);

	RockMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rock Mesh Component"));
	RockMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RockMeshComponent->SetupAttachment(DefaultSceneComponent);
	
	CrystalMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crystal Mesh Component"));
	CrystalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CrystalMeshComponent->SetupAttachment(RockMeshComponent);

	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Component"));
	BoxCollisionComponent->SetupAttachment(DefaultSceneComponent);
	BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	InteractWidgetComponent = CreateDefaultSubobject<UInteractWidgetComponent>(TEXT("Interact Widget Component"));
	InteractWidgetComponent->SetupAttachment(DefaultSceneComponent);

	InfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Info Widget Component"));
	InfoWidgetComponent->SetupAttachment(DefaultSceneComponent);

	HighlightComponent = CreateDefaultSubobject<UHighlightComponent>(TEXT("Highlight Component"));
}

// Called when the game starts or when spawned
void ACrystal::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmountCrystal = MaxAmountCrystal;
	HighlightComponent->SetMeshComponent(RockMeshComponent);
	HighlightComponent->SetWidgetComponent(InteractWidgetComponent);
	HighlightComponent->OnActivateHighlight().AddDynamic(this, &ACrystal::OnActivateHighlight);
	HighlightComponent->OnDeactivateHighlight().AddDynamic(this, &ACrystal::OnDeactivateHighlight);

	InfoWidgetComponent->SetVisibility(false);
	SetupCrystalClusterWidget();
}

void ACrystal::OnFinishCrystals()
{
	CurrentHarvestTime = TimeToHarvest;
	HighlightComponent->DeactivateHighlight();
	Execute_FinishInteraction(this, CurrentHarvestingActor);
	CrystalMeshComponent->SetVisibility(false);
}

void ACrystal::DrawDebugInfo() const
{
	if (CVarDebugCrystalCluster->GetBool())
	{
		const FString DebugInfo = FString::Printf(TEXT("Crystals: %d\nTime To Harvest: %.2f | Current Harvest Time: %.2f\nRemaining Harvest Time: %.2f"),
			CurrentAmountCrystal, TimeToHarvest, CurrentHarvestTime, GetRemainingHarvestTime());
		const FVector CrystalLocation = GetActorLocation();
		const FVector TextLocation = {CrystalLocation.X, CrystalLocation.Y, CrystalLocation.Z + 300.f};
		DrawDebugString(GetWorld(), TextLocation, DebugInfo, nullptr, FColor::Black, 0.1f);
	}
}

void ACrystal::SetupCrystalClusterWidget()
{
	if (InfoWidgetComponent != nullptr)
	{
		CrystalClusterWidgetRef = Cast<UCrystalClusterWidget>(InfoWidgetComponent->GetWidget());
		if (CrystalClusterWidgetRef != nullptr)
		{
			CrystalClusterWidgetRef->OnInitializeProperties(this, MaxAmountCrystal, TimeToHarvest);
		}
	}
}

// Called every frame
void ACrystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugInfo();
	if (bSomeoneHarvesting)
	{
		AddHarvestTime(DeltaTime);
	}
}

void ACrystal::StartInteraction_Implementation(AActor* ActorInteracting)
{
	if (!Execute_CanBeInteracted(this, ActorInteracting))
	{
		return;	
	}
	
	if (ActorInteracting == nullptr)
	{
		return;
	}
	
	UCrystalCollectorComponent* CrystalCollectorComponent = ActorInteracting->FindComponentByClass<UCrystalCollectorComponent>();
	if (CrystalCollectorComponent == nullptr)
	{
		return;
	}

	CurrentHarvestingActor = ActorInteracting;
	bSomeoneHarvesting = true;
	CrystalCollectorComponent->StartHarvesting(this);
	if (CrystalClusterWidgetRef != nullptr)
	{
		CrystalClusterWidgetRef->OnStartHarvesting(ActorInteracting);
	}
}

void ACrystal::FinishInteraction_Implementation(AActor* ActorInteracting)
{
	if (ActorInteracting == nullptr)
	{
		return;
	}
	
	UCrystalCollectorComponent* CrystalCollectorComponent = ActorInteracting->FindComponentByClass<UCrystalCollectorComponent>();
	if (CrystalCollectorComponent == nullptr)
	{
		return;
	}

	CrystalCollectorComponent->StopHarvesting();
	CurrentHarvestingActor = nullptr;
	bSomeoneHarvesting = false;
	if (CrystalClusterWidgetRef != nullptr)
	{
		CrystalClusterWidgetRef->OnStopHarvesting(ActorInteracting);
	}
}

bool ACrystal::CanBeInteracted_Implementation(AActor* ActorInteracting) const
{
	// TODO Could also check if actor has crystal collector component
	return HasCrystals();
}

int32 ACrystal::Harvest(const int32 RequestedCrystals)
{
	// Critical section?
	const int32 CollectedCrystals = (CurrentAmountCrystal - RequestedCrystals < 0)? CurrentAmountCrystal : RequestedCrystals;
	CurrentAmountCrystal -= CollectedCrystals;
	if (CurrentAmountCrystal <= 0)
	{
		OnFinishCrystals();
	}

	if (CrystalClusterWidgetRef != nullptr)
	{
		CrystalClusterWidgetRef->OnHarvestCrystals(CollectedCrystals, CurrentAmountCrystal);
	}
	
	return CollectedCrystals;
}

void ACrystal::AddHarvestTime(const float DeltaTime)
{
	CurrentHarvestTime = FMath::Clamp(CurrentHarvestTime + DeltaTime, 0.f, TimeToHarvest);
}

void ACrystal::OnActivateHighlight()
{
	InfoWidgetComponent->SetVisibility(true);
}

void ACrystal::OnDeactivateHighlight()
{
	InfoWidgetComponent->SetVisibility(false);
}

