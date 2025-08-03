#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FTutorialStart);
DECLARE_MULTICAST_DELEGATE(FTutorialEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAllTutorialEnd);
DECLARE_DELEGATE_RetVal(bool, FTutorialCondition);

USTRUCT()
struct FTutorial
{
	GENERATED_BODY()

public:
	/** 튜토리얼 시작 시 호출되는 델리게이트 */
	FTutorialStart TutorialStartDelegate;

	/** 튜토리얼 종료 시 호출되는 델리게이트 */
	FTutorialEnd TutorialEndDelegate;

	/** 튜토리얼을 만족하는 조건 */
	FTutorialCondition TutorialCondition;
	
};

UCLASS()
class SEVENDAYS_API ATutorialManager : public AActor
{
	GENERATED_BODY()

public:	
	ATutorialManager();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	/** 모든 튜토리얼이 끝났을 때 호출되는 델리게이트 */
	UPROPERTY(BlueprintAssignable)
	FAllTutorialEnd OnAllTutorialEndDelegate;
	
public:
	/** 튜토리얼 시작 함수 */
	UFUNCTION(BlueprintCallable)
	void StartTutorial();

	/** 다음 튜토리얼로 넘어가는 함수 */
	void NextTutorial();

	/** 현재 튜토리얼을 제거하는 함수 */
	void RemoveTutorial();

private:
	/** WASD 튜토리얼 조건을 확인하는 함수 */
	bool WASDTutorialCondition();

	/** 사진 촬영 튜토리얼 조건을 확인하는 함수*/
	bool PictureTutorialCondition();

	/** 문 열기 튜토리얼 조건을 확인하는 함수 */
	bool DoorOpenTutorialCondition();

public:
	UPROPERTY()
	FTutorial WASDTutorial;

	UPROPERTY()
	FTutorial PictureTutorial;

	UPROPERTY()
	FTutorial DoorOpenTutorial;


private:
	UPROPERTY()
	TArray<FTutorial> TutorialEventList;

	uint8 bIsTutorialActive : 1 = false;
	uint8 bPictureInputDown : 1 = false;
	uint8 bDoorOpenInputDown : 1 = false;
	
	FTimerHandle TutorialHandle;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetPictureInputDown(const bool bDown) { bPictureInputDown = bDown; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDoorOpenInputDown(const bool bDown) { bDoorOpenInputDown = bDown; }

};
