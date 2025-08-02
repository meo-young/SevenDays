#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class UCrosshairWidget;
class UCameraComponent;
class IInteractable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SEVENDAYS_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

public:
	/** 전방에 LineTracing을 실시하는 함수입니다. */
	void LineTraceForward();

	/** 상호작용을 시작하는 함수입니다. */
	void StartInteraction();

	/** 카메라 촬영 효과를 연출하는 함수입니다. */
	UFUNCTION()
	void ShowExposureEffect();

	/** 카메라 노출 효과를 업데이트하는 함수입니다. */
	void UpdateExposure();

protected:
	/** CrossHair 위젯 클래스입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCrosshairWidget> CrosshairWidgetClass;

	/** CrossHair 위젯 인스턴스입니다. */
	UPROPERTY()
	UCrosshairWidget* CrosshairWidgetInstance;

private:
	FHitResult InteractionHitResult;
	FCollisionQueryParams QueryParams;
	FTimerHandle ExposureTimerHandle;
	FTimerHandle InteractionTimerHandle;

	/** 상호작용 가능한 오브젝트입니다. */
	IInteractable* InteractableObject = nullptr;

	/** 플레이어 카메라 컴포넌트입니다. */
	UPROPERTY()
	UCameraComponent* PlayerCameraComponent;

	/** Exposure 효과를 위한 보간 타이머입니다. */
	float ExposureLerpTimer = 0.0f;

	/** 현재 노출 값입니다. */
	float CurrentExposure = 0.0f;

	/** 상호작용이 가능한지 판별하는 변수입니다. */
	uint8 bCanInteract : 1 = false;
	
};
