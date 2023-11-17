// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/TractorPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UI/TractorHud.h"
#include "Characters/StellarBaseCharacter.h"
#include "Components/AbilityComponent.h"
#include "Components/CounterMeasureComponent.h"
#include "Components/CrystalCollectorComponent.h"
#include "Components/AbilityComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameStates/StellarHarvestGameState.h"

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
	SetupEvents();
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

			EnhancedInputComponent->BindAction(RequestCounterMeasureAction, ETriggerEvent::Triggered, this, &ATractorPlayerController::RequestCounterMeasure);
			
			EnhancedInputComponent->BindAction(RequestPrimaryAbilityAction, ETriggerEvent::Triggered, this, &ATractorPlayerController::RequestPrimaryAbility);
			EnhancedInputComponent->BindAction(RequestSecondaryAbilityAction, ETriggerEvent::Triggered, this, &ATractorPlayerController::RequestSecondaryAbility);

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

void ATractorPlayerController::RequestCounterMeasure(const FInputActionInstance&)
{
	if (TractorRef != nullptr)
	{
		UCounterMeasureComponent* CounterMeasureComponent = TractorRef->FindComponentByClass<UCounterMeasureComponent>();
		if (CounterMeasureComponent != nullptr)
		{
			CounterMeasureComponent->UseCounterMeasure();
		}
	}
}

void ATractorPlayerController::RequestPrimaryAbility(const FInputActionInstance&)
{
	if (TractorRef != nullptr)
	{
		UAbilityComponent* AbilityComponent = TractorRef->FindComponentByClass<UAbilityComponent>();
		if (AbilityComponent != nullptr)
		{
			AbilityComponent->ApplyPrimaryAbility();
		}
	}
}

void ATractorPlayerController::RequestSecondaryAbility(const FInputActionInstance&)
{
	if (TractorRef != nullptr)
	{
		UAbilityComponent* AbilityComponent = TractorRef->FindComponentByClass<UAbilityComponent>();
		if (AbilityComponent != nullptr)
		{
			AbilityComponent->ApplySecondaryAbility();
		}
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

void ATractorPlayerController::StartGame()
{
	if (HUDWidgetRef != nullptr)
	{
		HUDWidgetRef->AddToPlayerScreen();
	}
}

void ATractorPlayerController::NewScoreGoal() const
{
	if (HUDWidgetRef != nullptr)
	{
		HUDWidgetRef->OnNewScoreGoal();
	}
}

void ATractorPlayerController::ReachedScoreGoal()
{
	if (HUDWidgetRef != nullptr)
	{
		HUDWidgetRef->OnReachScoreGoal();
	}
}

void ATractorPlayerController::GameOver(const bool bIsWinner)
{
	const FInputModeUIOnly InputUIMode;
	SetInputMode(InputUIMode);
	SetShowMouseCursor(true);
	
	if (HUDWidgetRef == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player HUD or GameOver are nullptr"));
		return;
	}

	HUDWidgetRef->OnGameOver(bIsWinner);
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
				const float MovementRatio = (bScaleMovementBasedOnDistance)? CalculateMouseMovementRatio() : 1.f;
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

void ATractorPlayerController::SetupEvents()
{
	UCrystalCollectorComponent* CrystalCollectorComponent = GetPawn()->FindComponentByClass<UCrystalCollectorComponent>();
	if (CrystalCollectorComponent)
	{
		CrystalCollectorComponent->OnCollectCrystals().AddDynamic(this, &ATractorPlayerController::OnCollectCrystals);
	}

	UCounterMeasureComponent* CounterMeasureComponent = GetPawn()->FindComponentByClass<UCounterMeasureComponent>();
	if (CounterMeasureComponent)
	{
		CounterMeasureComponent->OnUseCounterMeasure().AddDynamic(this, &ATractorPlayerController::OnUseCounterMeasure);
		CounterMeasureComponent->OnEnemiesInRange().AddDynamic(this, &ATractorPlayerController::OnEnemyInRange);
		CounterMeasureComponent->OnLoseEnemiesInRange().AddDynamic(this, &ATractorPlayerController::OnLoseEnemiesInRange);
		CounterMeasureComponent->OnCooldownFinished().AddDynamic(this, &ATractorPlayerController::OnCounterMeasureCooldownFinished);
	}

	UAbilityComponent* AbilityComponent = GetPawn()->FindComponentByClass<UAbilityComponent>();
	if (AbilityComponent)
	{
		AbilityComponent->OnPrimaryAbilityApplied.AddDynamic(this, &ATractorPlayerController::OnPrimaryAbilityApplied);
		AbilityComponent->OnPrimaryAbilityAvailable.AddDynamic(this, &ATractorPlayerController::OnPrimaryAbilityAvailable);
		AbilityComponent->OnPrimaryAbilityRemoved.AddDynamic(this, &ATractorPlayerController::OnPrimaryAbilityRemoved);
		
		AbilityComponent->OnSecondaryAbilityApplied.AddDynamic(this, &ATractorPlayerController::OnSecondaryAbilityApplied);
		AbilityComponent->OnSecondaryAbilityAvailable.AddDynamic(this, &ATractorPlayerController::OnSecondaryAbilityAvailable);
		AbilityComponent->OnSecondaryAbilityRemoved.AddDynamic(this, &ATractorPlayerController::OnSecondaryAbilityRemoved);
	}
}

void ATractorPlayerController::OnCollectCrystals(const int32 CollectedCrystals, const int32 TotalCrystals)
{
	if (HUDWidgetRef != nullptr)
	{
		HUDWidgetRef->OnCollectCrystals(CollectedCrystals, TotalCrystals);
	}

	AStellarHarvestGameState* GameState = Cast<AStellarHarvestGameState>(GetWorld()->GetGameState());
	if (GameState != nullptr)
	{
		GameState->AddScore(this, CollectedCrystals);
	}
}

void ATractorPlayerController::OnUseCounterMeasure(const int32 CurrentAmount)
{
	if (HUDWidgetRef != nullptr)
	{
		HUDWidgetRef->OnUseCounterMeasure(CurrentAmount);
	}
}

void ATractorPlayerController::OnEnemyInRange()
{
	if (HUDWidgetRef != nullptr)
	{
		HUDWidgetRef->OnEnemyInRange();
	}
}

void ATractorPlayerController::OnLoseEnemiesInRange()
{
	if (HUDWidgetRef != nullptr)
	{
		HUDWidgetRef->OnLoseEnemiesInRange();
	}
}

void ATractorPlayerController::OnCounterMeasureCooldownFinished()
{
	if (HUDWidgetRef != nullptr)
	{
		HUDWidgetRef->OnCounterMeasureCooldownFinished();
	}
}

void ATractorPlayerController::OnPrimaryAbilityApplied(const UEffectAbility* Ability)
{
	if (HUDWidgetRef != nullptr)
	{
		HUDWidgetRef->OnPrimaryAbilityApplied(Ability);
	}
}

void ATractorPlayerController::OnPrimaryAbilityRemoved(const UEffectAbility* Ability)
{
	if (HUDWidgetRef != nullptr)
	{
		HUDWidgetRef->OnPrimaryAbilityRemoved(Ability);
	}
}


void ATractorPlayerController::OnPrimaryAbilityAvailable(const UEffectAbility* Ability)
{
	if (HUDWidgetRef != nullptr)
	{
		HUDWidgetRef->OnPrimaryAbilityAvailable(Ability);
	}
}

void ATractorPlayerController::OnSecondaryAbilityApplied(const UEffectAbility* Ability)
{
	if (HUDWidgetRef != nullptr)
	{
		HUDWidgetRef->OnSecondaryAbilityApplied(Ability);
	}
}

void ATractorPlayerController::OnSecondaryAbilityRemoved(const UEffectAbility* Ability)
{
	if (HUDWidgetRef != nullptr)
	{
		HUDWidgetRef->OnSecondaryAbilityRemoved(Ability);
	}
}

void ATractorPlayerController::OnSecondaryAbilityAvailable(const UEffectAbility* Ability)
{
	if (HUDWidgetRef != nullptr)
	{
		HUDWidgetRef->OnSecondaryAbilityAvailable(Ability);
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

