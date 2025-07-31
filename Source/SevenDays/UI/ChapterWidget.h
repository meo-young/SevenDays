#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChapterWidget.generated.h"

UCLASS()
class SEVENDAYS_API UChapterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
 	/** 현재 스테이지 정보를 화면에 표시하는 함수입니다. */
	UFUNCTION(BlueprintImplementableEvent)
	void ShowEyeCatch(const uint8& CurrentChapterIndex);
	
};
