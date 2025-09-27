// Copyright Mattman


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true; // When a change is made on the server, the change replicates down to clients
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext); // assert

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	check(Subsystem); // assert
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;


	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	 * Line trace from cursor. Several scenarios.
	 * A. LastActor is null && ThisActor is null.
	 *		- Do nothing.
	 *		
	 * B. LastActor is null && ThisActor is non-null.
	 *		- Call highlight on ThisActor.
	 *		
	 * C. LastActor is non-null && ThisActor is null.
	 *		- Call UnHighlight on LastActor.
	 *		
	 * D. Last Actor is non-null && This Actor is non-null && LastActor != ThisActor.
	 *		- Call UnHighlight on LastActor.
	 *		- Call Highlight on ThisActor.
	 *
	 * E. Last Actor is non-null && This Actor is non-null && LastActor == ThisActor.
	 *		- Do nothing.
	 */
	if (!LastActor && ThisActor)
	{
		// Case B
		ThisActor->HighlightActor();
	}
	else if (LastActor && !ThisActor)
	{
		// Case C
		LastActor->UnHighlightActor();
	}
	else if (LastActor && ThisActor && LastActor != ThisActor)
	{
		// Case D
		LastActor->UnHighlightActor();
		ThisActor->HighlightActor();
	}
}
