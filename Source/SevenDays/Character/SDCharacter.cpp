#include "Character/SDCharacter.h"
#include "EnhancedInputComponent.h"


void ASDCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
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
	UE_LOG(LogTemp, Warning, TEXT("DoInteract called!"));
}
