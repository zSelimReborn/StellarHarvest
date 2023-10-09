// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/StellarBaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
	
}

// Called every frame
void AStellarBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

FRotator AStellarBaseCharacter::GetActualViewRotation() const
{
	FVector CameraLocation;
	FRotator CCameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CCameraRotation);

	return CCameraRotation;
}
