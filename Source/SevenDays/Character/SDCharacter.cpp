#include "Character/SDCharacter.h"
#include "EnhancedInputComponent.h"
#include "SevenDays.h"
#include "Camera/CameraComponent.h"
#include "Components/InteractionComponent.h"
#include "Core/SDGameModeBase.h"
#include "Define/DefineClass.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/SoundSubsystem.h"

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

		if (DoorAction)
		{
			EnhancedInputComponent->BindAction(DoorAction, ETriggerEvent::Started, this, &ThisClass::OpenDoor);
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

void ASDCharacter::SetDeadMode_Implementation()
{
	DisablePlayerInput();
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->GetAnimInstance()->EnableUpdateAnimation(false);
	CameraComponent->bUsePawnControlRotation = false;
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = true;
	SetBattery(0);

	FTimerHandle DeadSoundHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadSoundHandle, FTimerDelegate::CreateLambda([this]()
	{
		USoundSubsystem* SoundSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USoundSubsystem>();
		SoundSubsystem->PlaySFX(ESFX::BodyFall, GetActorLocation() + FVector(0.0f, 0.0f, 10.0f));
	}), 0.5f, false);

	FTimerHandle SimulatedPhysicsHandle;
	GetWorld()->GetTimerManager().SetTimer(SimulatedPhysicsHandle, FTimerDelegate::CreateLambda([this]()
	{
		CameraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}), 2.0f, false);
}

void ASDCharacter::SetNormalMode_Implementation()
{
	Destroy();
	GetWorld()->GetAuthGameMode()->RestartPlayer(GetController());

	SetBattery(100);
}

void ASDCharacter::MoveInput(const FInputActionValue& Value)
{
	if (!bIsEnableMove) return;
	
	const FVector2D MovementValue = Value.Get<FVector2D>();
	if (MovementValue.IsNearlyZero())
	{
		return;
	}

	DoMove(MovementValue.X, MovementValue.Y);
}

void ASDCharacter::LookInput(const FInputActionValue& Value)
{
	if (!bIsEnableMove) return;
	
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
	if (!bIsEnableInteract) return;
	
	InteractionComponent->LineTraceForward();
}

void ASDCharacter::OpenDoor()
{
	if (!bIsEnableDoorOpen) return;
	
	InteractionComponent->DoorInteraction();
}
