// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/TractorPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UI/TractorHud.h"
#include "Characters/StellarBaseCharacter.h"

static TAutoConsoleVariable<bool> CVarDebugMouseMove(
	TEXT("StellarHarvest.Tractor.DebugMouseMove"),
	false,
	TEXT("Show debug info about moving using mouse."),
	ECVF_Default
);


void ATractorPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MinMouseDistanceSquared = MinMouseDistance * MinMouseDistance;
}

void ATractorPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TractorRef = Cast<AStellarBaseCharacter>(InPawn);
	InitializeHUD();
}

void ATractorPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
		if (EnhancedInputComponent)
		{
			EnhancedInputComponent->BindAction(AnalogMoveAction, ETriggerEvent::Triggered, this, &ATractorPlayerController::RequestAnalogMove);
			EnhancedInputComponent->BindAction(RequestMouseMoveAction, ETriggerEvent::Triggered, this, &ATractorPlayerController::RequestMovementUsingMouse);
			EnhancedInputComponent->BindAction(RequestInteractAction, ETriggerEvent::Started, this, &ATractorPlayerController::RequestStartInteraction);
			EnhancedInputComponent->BindAction(RequestInteractAction, ETriggerEvent::Completed, this, &ATractorPlayerController::RequestFinishInteraction);

			InitializeMappingContext();
		}
	}
}

void ATractorPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MoveUsingMouse();	
}

void ATractorPlayerController::RequestAnalogMove(const FInputActionInstance& Value)
{
	DisableMouseMove();
	const FVector2D AxisValue = Value.GetValue().Get<FVector2D>();
	if (TractorRef != nullptr)
	{
		TractorRef->RequestMove(AxisValue);
	}
}

void ATractorPlayerController::RequestMovementUsingMouse(const FInputActionInstance&)
{
	EnableMouseMove();
}

void ATractorPlayerController::RequestStartInteraction(const FInputActionInstance&)
{
	if (TractorRef != nullptr)
	{
		TractorRef->RequestStartInteraction();
	}
}

void ATractorPlayerController::RequestFinishInteraction(const FInputActionInstance&)
{
	if (TractorRef != nullptr)
	{
		TractorRef->RequestFinishInteraction();
	}
}

void ATractorPlayerController::EnableMouseMove()
{
	bIsUsingMouseMove = true;
}

void ATractorPlayerController::DisableMouseMove()
{
	bIsUsingMouseMove = false;
}

void ATractorPlayerController::MoveUsingMouse()
{
	if (bUseMouseMove && bIsUsingMouseMove && TractorRef != nullptr)
	{
		FHitResult MouseTraceResult;
		if (GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, MouseTraceResult))
		{
			const FVector ImpactLocation = MouseTraceResult.Location;
			const FVector DirectionToPoint = ImpactLocation - GetPawn()->GetActorLocation();
			const float DirectionToPointSquaredLength = DirectionToPoint.SquaredLength();

			if (DirectionToPointSquaredLength >= MinMouseDistanceSquared)
			{
				const float MovementRatio = CalculateMouseMovementRatio();
				TractorRef->RequestMove(DirectionToPoint.GetSafeNormal(), MovementRatio);

				if (CVarDebugMouseMove->GetBool())
				{
					GEngine->AddOnScreenDebugMessage(
						-1,
						0.f,
						FColor::Blue,
						FString::Printf(TEXT("TRACTOR -> Movement Ratio: %.2f"), MovementRatio),
						true
					);
				}
			}

			if (CVarDebugMouseMove->GetBool())
			{
				DrawDebugPoint(GetWorld(), ImpactLocation, 5.f, FColor::Red, false, 0.3f);
				DrawDebugDirectionalArrow(GetWorld(), GetPawn()->GetActorLocation(), ImpactLocation, 5.f, FColor::Blue, false, 0.3f);
			}
		}
	}
}

void ATractorPlayerController::InitializeMappingContext()
{
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (EnhancedInputLocalPlayerSubsystem)
	{
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ATractorPlayerController::InitializeHUD()
{
	if (HUDWidgetClass)
	{
		HUDWidgetRef = CreateWidget<UTractorHud>(this, HUDWidgetClass);
		if (HUDWidgetRef != nullptr)
		{
			HUDWidgetRef->InitializeWidget(this);
			HUDWidgetRef->AddToPlayerScreen();
		}
	}
}

float ATractorPlayerController::CalculateMouseMovementRatio() const
{
	float MouseX, MouseY;
	if (!GetMousePosition(MouseX, MouseY))
	{
		return 0.f;
	}

	int32 ViewportX, ViewportY;
	GetViewportSize(ViewportX, ViewportY);
	const FVector2D ScreenCenter{ViewportX/2.f, ViewportY/2.f};
	
	const float XDistanceToCenter = FMath::Abs(MouseX - ScreenCenter.X);
	const float YDistanceToCenter = FMath::Abs(MouseY - ScreenCenter.Y);
	const float XPercentage = FMath::Abs(XDistanceToCenter / ScreenCenter.X);
	const float YPercentage = FMath::Abs(YDistanceToCenter / ScreenCenter.Y);

	return FMath::Max(XPercentage, YPercentage);
}
