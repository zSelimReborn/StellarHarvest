// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/StellarBaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/SearchInteractableComponent.h"
#include "Components/HighlightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Interactable.h"

// Sets default values
AStellarBaseCharacter::AStellarBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(CameraBoom);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AStellarBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitialWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

// Called every frame
void AStellarBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStellarBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SearchItemsComponent = FindComponentByClass<USearchInteractableComponent>();
	InitializeObservers();
}

float AStellarBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	APlayerController* PC = GetController<APlayerController>();
	if (PC != nullptr)
	{
		PC->RestartLevel();
	}
	
	return DamageAmount;
}

UAISense_Sight::EVisibilityResult AStellarBaseCharacter::CanBeSeenFrom(const FCanBeSeenFromContext& Context,
	FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested,
	float& OutSightStrength, int32* UserData, const FOnPendingVisibilityQueryProcessedDelegate* Delegate)
{
	return (bIsSightVisible)? UAISense_Sight::EVisibilityResult::Visible : UAISense_Sight::EVisibilityResult::NotVisible;
}

void AStellarBaseCharacter::SetVisible()
{
	bIsSightVisible = true;
}

void AStellarBaseCharacter::SetInvisible()
{
	bIsSightVisible = false;
}

void AStellarBaseCharacter::RequestMove(const FVector2D& AxisRatio)
{
	const float ForwardAxisValue = AxisRatio.X;
	const float RightAxisValue = AxisRatio.Y;

	const FRotator CameraRotation = GetActualViewRotation();
	const FRotator CurrentRotation{0.f, CameraRotation.Yaw, 0.f};
	const FRotationMatrix CurrentRotationMatrix{CurrentRotation};
	
	AddMovementInput(CurrentRotationMatrix.GetScaledAxis(EAxis::X), ForwardAxisValue);
	AddMovementInput(CurrentRotationMatrix.GetScaledAxis(EAxis::Y), RightAxisValue);

	// TODO Engine sound?
}

void AStellarBaseCharacter::RequestMove(const FVector& Direction, const float AxisRatio)
{
	AddMovementInput(Direction, AxisRatio);
	
	// TODO Engine sound?
}

void AStellarBaseCharacter::RequestStartInteraction()
{
	AActor* SelectedInteractable = SearchItemsComponent->GetFoundItem();
	if (SelectedInteractable != nullptr && SelectedInteractable->Implements<UInteractable>())
	{
		IInteractable::Execute_StartInteraction(SelectedInteractable, this);
	}
}

void AStellarBaseCharacter::RequestFinishInteraction()
{
	AActor* SelectedInteractable = SearchItemsComponent->GetFoundItem();
	if (SelectedInteractable != nullptr && SelectedInteractable->Implements<UInteractable>())
	{
		IInteractable::Execute_FinishInteraction(SelectedInteractable, this);
	}
}

void AStellarBaseCharacter::RequestSprint() const
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AStellarBaseCharacter::RequestWalk() const
{
	GetCharacterMovement()->MaxWalkSpeed = InitialWalkSpeed;
}

FRotator AStellarBaseCharacter::GetActualViewRotation() const
{
	FVector CameraLocation;
	FRotator CCameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CCameraRotation);

	return CCameraRotation;
}

void AStellarBaseCharacter::InitializeObservers() {}
