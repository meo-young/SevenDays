#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Define/DefineClass.h"
#include "MissionWidget.generated.h"

UCLASS()
class SEVENDAYS_API UMissionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

public:
	/** 현재 스테이지의 미션 정보를 화면에 표시하는 함수입니다. */
	UFUNCTION(BlueprintImplementableEvent)
	void ShowMissionWidget(const EMissionType& MissionTypes);

	/** 현재 스테이지의 미션 정보를 화면에서 제거하는 함수입니다. */
	UFUNCTION(BlueprintImplementableEvent)
	void RemoveMissionWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void CreateMissions();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowWASDTutorialWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveWASDTutorialWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowPictureTutorialWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void RemovePictureTutorialWidget();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowDoorOpenTutorialWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveDoorOpenTutorialWidget();
};
