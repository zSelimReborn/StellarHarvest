// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Interact/Crystal.h"

#include "Components/BoxComponent.h"
#include "Components/HighlightComponent.h"
#include "Components/WidgetComponent.h"

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

	InteractWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interact Widget Component"));
	InteractWidgetComponent->SetupAttachment(DefaultSceneComponent);

	HighlightComponent = CreateDefaultSubobject<UHighlightComponent>(TEXT("Highlight Component"));
}

// Called when the game starts or when spawned
void ACrystal::BeginPlay()
{
	Super::BeginPlay();
	HighlightComponent->SetMeshComponent(RockMeshComponent);
	HighlightComponent->SetWidgetComponent(InteractWidgetComponent);
}

// Called every frame
void ACrystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACrystal::StartInteraction_Implementation(AActor* ActorInteracting)
{
	UE_LOG(LogTemp, Error, TEXT("Start interaction %s!"), *GetActorLabel());
}

void ACrystal::FinishInteraction_Implementation(AActor* ActorInteracting)
{
	UE_LOG(LogTemp, Error, TEXT("Finish interaction %s!"), *GetActorLabel());
}

