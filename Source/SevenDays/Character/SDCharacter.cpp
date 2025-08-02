#include "Character/SDCharacter.h"
#include "EnhancedInputComponent.h"
#include "SevenDays.h"
#include "Camera/CameraComponent.h"
#include "Components/InteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASDCharacter::ASDCharacter()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetMesh(), FName("head"));
	CameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	CameraComponent->bUsePawnControlRotation = true;
	CameraComponent->bEnableFirstPersonFieldOfView = true;
	CameraComponent->bEnableFirstPersonScale = true;
	CameraComponent->FirstPersonFieldOfView = 70.0f;
	CameraComponent->FirstPersonScale = 0.6f;

	GetCharacterMovement()->MaxWalkSpeed = 100.0f;

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
}

void ASDCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);
		}

		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::DoInteract);
		}
	}
}

void ASDCharacter::DisablePlayerInput()
{
	DisableInput(Cast<APlayerController>(GetController()));
}

void ASDCharacter::EnablePlayerInput()
{
	EnableInput(Cast<APlayerController>(GetController()));
}

void ASDCharacter::MoveInput(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();
	if (MovementValue.IsNearlyZero())
	{
		return;
	}

	DoMove(MovementValue.X, MovementValue.Y);
}

void ASDCharacter::LookInput(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();
	if (LookValue.IsNearlyZero())
	{
		return;
	}

	DoLook(LookValue.Y, LookValue.X);
}

void ASDCharacter::DoMove(const float Forward, const float Right)
{
	if (GetController())
	{
		AddMovementInput(GetActorForwardVector(), Forward);
		AddMovementInput(GetActorRightVector(), Right);
	}
}

void ASDCharacter::DoLook(const float Pitch, const float Yaw)
{
	if (GetController())
	{
		AddControllerPitchInput(Pitch);
		AddControllerYawInput(Yaw);	
	}
}

void ASDCharacter::DoInteract()
{
	InteractionComponent->LineTraceForward();
}
