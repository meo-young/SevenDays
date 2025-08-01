#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Define/DefineClass.h"
#include "MissionWidget.generated.h"

UCLASS(BlueprintType)
class SEVENDAYS_API UMissionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 현재 스테이지의 미션 정보를 화면에 표시하는 함수입니다. */
	UFUNCTION(BlueprintImplementableEvent)
	void ShowMissionWidget(const EMissionType& MissionTypes);
	
};
