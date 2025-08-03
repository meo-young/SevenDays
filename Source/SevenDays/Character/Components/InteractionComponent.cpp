#include "Character/Components/InteractionComponent.h"
#include "SevenDays.h"
#include "Camera/CameraComponent.h"
#include "Character/SDCharacter.h"
#include "Define/DefineClass.h"
#include "Interface/Interactable.h"
#include "Subsystem/SoundSubsystem.h"
#include "Subsystem/StageSubsystem.h"
#include "UI/CrosshairWidget.h"
#include "UI/MissionWidget.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	ASDCharacter* Player = Cast<ASDCharacter>(GetOwner());

	// 플레이어 카메라의 PostProcessSettings를 설정
	PlayerCameraComponent = Player->GetCameraComponent();
	PlayerCameraComponent->PostProcessSettings.bOverride_AutoExposureBias = true;

	if (CrosshairWidgetClass)
	{
		CrosshairWidgetInstance = CreateWidget<UCrosshairWidget>(GetWorld(), CrosshairWidgetClass);
	}
}

void UInteractionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 카메라의 현재 위치에서 정면을 기준으로 LineTracing을 수행합니다.
	const UCameraComponent* CameraComponent = GetOwner()->FindComponentByClass<UCameraComponent>();
	const FVector CurrentLocation = CameraComponent->GetComponentLocation();
	const FVector ForwardVector = CameraComponent->GetForwardVector();
	const FVector TargetLocation = CurrentLocation + (ForwardVector * 250.0f);

	// LineTracing 성공 여부를 저장합니다.
	bCanDoorInteract = GetWorld()->LineTraceSingleByChannel(
		DoorHitResult,
		CurrentLocation,
		TargetLocation,
		ECC_DOOR,
		QueryParams
		);

	if (!bCanDoorInteract)
	{
		DoorHitResult = FHitResult();
	}
}


void UInteractionComponent::LineTraceForward()
{
	// 플레이어 입력을 비활성화 합니다.
	ASDCharacter* Player = Cast<ASDCharacter>(GetOwner());
	Player->DisablePlayerInput();
	bIsInteractionStarted = true;

	// 카메라 셔터 소리를 재생합니다.
	USoundSubsystem* SoundSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USoundSubsystem>();
	SoundSubsystem->PlaySFX(ESFX::Camera_Shutter, Player->GetActorLocation());

	// Crosshair 위젯을 로딩 상태로 변경합니다.
	CrosshairWidgetInstance->LoadingCrosshair();
	
	// 카메라의 현재 위치에서 정면을 기준으로 LineTracing을 수행합니다.
	const UCameraComponent* CameraComponent = GetOwner()->FindComponentByClass<UCameraComponent>();
	const FVector CurrentLocation = CameraComponent->GetComponentLocation();
	const FVector ForwardVector = CameraComponent->GetForwardVector();
	const FVector TargetLocation = CurrentLocation + (ForwardVector * 400.0f);

	// LineTracing 성공 여부를 저장합니다.
	bCanInteract = GetWorld()->LineTraceSingleByChannel(
		InteractionHitResult,
		CurrentLocation,
		TargetLocation,
		ECC_INTERACTION,
		QueryParams
		);

#if WITH_EDITOR
	DrawDebugLine(GetWorld(), CurrentLocation, TargetLocation, FColor::Red, false, 3.0f, 0, 1.0f);
#endif

	// 2초 후 상호작용 로직을 수행합니다.
	GetOwner()->GetWorldTimerManager().SetTimer(InteractionTimerHandle, this, &ThisClass::StartInteraction, 2.0f, false);
	
}

void UInteractionComponent::StartInteraction()
{
	// 플레이어 입력을 다시 활성화합니다.
	ASDCharacter* Player = Cast<ASDCharacter>(GetOwner());
	Player->EnablePlayerInput();
	bIsInteractionStarted = false;

	// 일반 Crosshair로 변경합니다.
	CrosshairWidgetInstance->NormalCrosshair();

	// 스테이지가 시작되지 않았다면 함수를 종료합니다.
	UStageSubsystem* StageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UStageSubsystem>();
	if (!StageSubsystem->IsStageStarted())
	{
		return;
	}
	
	// 상호작용이 가능한 오브젝트라면 Interact 함수를 호출합니다.
	USoundSubsystem* SoundSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USoundSubsystem>();
	if (bCanInteract)
	{
		// 카메라 플래시 소리를 재생합니다.
		SoundSubsystem->PlaySFX(ESFX::Camera_Flash, Player->GetActorLocation());

		// 카메라 연출을 출력합니다.
		ShowExposureEffect();
		
		InteractableObject = Cast<IInteractable>(InteractionHitResult.GetActor());
		IInteractable::Execute_Interact(InteractableObject->_getUObject());

		StageSubsystem->GetMissionWidget()->RemoveMissionWidget();

		bCanInteract = false;
	}
	else
	{
		SoundSubsystem->PlaySFX(ESFX::Camera_WrongShutter, Player->GetActorLocation());
		if (!Player->IsBatteryActive()) return;
		
		Player->SetBattery(FMath::Clamp(Player->GetBattery() - 40, 0, 100));
	}
}

void UInteractionComponent::DoorInteraction()
{
	if (bCanDoorInteract)
	{
		InteractableObject = Cast<IInteractable>(DoorHitResult.GetActor());
		IInteractable::Execute_Interact(InteractableObject->_getUObject());

		UStageSubsystem* StageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UStageSubsystem>();
		StageSubsystem->SetStageSucceeded(false);

		bCanDoorInteract = false;
	}
}

void UInteractionComponent::ShowExposureEffect()
{
	CurrentExposure = 5.0f;

	// Bloom 초기값 설정
	PlayerCameraComponent->PostProcessSettings.bOverride_AutoExposureBias = true;
	PlayerCameraComponent->PostProcessSettings.AutoExposureBias = CurrentExposure;

	// 타이머 설정
	GetOwner()->GetWorldTimerManager().SetTimer(ExposureTimerHandle, this, &ThisClass::UpdateExposure, 0.016f, true);
}

void UInteractionComponent::UpdateExposure()
{
	ExposureLerpTimer += 0.016f;
	const float Alpha = FMath::Clamp(ExposureLerpTimer / 2.0f, 0.0f, 1.0f);
    
	CurrentExposure = FMath::Lerp(5.0f, 1.0f, Alpha);
	PlayerCameraComponent->PostProcessSettings.AutoExposureBias = CurrentExposure;
	
	if (Alpha >= 1.0f)
	{
		ExposureLerpTimer = 0.0f;
		PlayerCameraComponent->PostProcessSettings.AutoExposureBias = 0.25f;
		PlayerCameraComponent->PostProcessSettings.bOverride_AutoExposureBias = false;

		GetOwner()->GetWorldTimerManager().ClearTimer(ExposureTimerHandle);
	}
}
