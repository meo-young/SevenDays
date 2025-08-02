#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SDCharacter.generated.h"

class UInteractionComponent;
class UCameraComponent;
class UChapterWidget;
struct FInputActionValue;
class UInputAction;

UCLASS()
class SEVENDAYS_API ASDCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASDCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** 플레이어의 입력을 비활성화 하는 함수입니다.*/
	UFUNCTION(BlueprintCallable)
	void DisablePlayerInput();

	/** 플레이어의 입력을 활성화 하는 함수입니다.*/
	UFUNCTION(BlueprintCallable)
	void EnablePlayerInput();

protected:
	/** W,A,S,D 키 입력을 처리하기 위한 함수입니다. */
	void MoveInput(const FInputActionValue& Value);

	/** 마우스 움직임 입력을 처리하기 위한 함수입니다. */
	void LookInput(const FInputActionValue& Value);

	/** 캐릭터를 이동시키는 함수입니다. */
	void DoMove(const float Forward, const float Right);

	/** 캐릭터의 시점을 조정하는 함수입니다. */
	void DoLook(const float Pitch, const float Yaw);

	/** 상호작용을 수행하는 함수입니다. */
	void DoInteract();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UInteractionComponent* InteractionComponent;

public:
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }

};
