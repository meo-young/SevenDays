#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StageSubsystem.generated.h"

class UChapterWidget;

UCLASS(Blueprintable)
class SEVENDAYS_API UStageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UStageSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	/**  ChapterWidget이 화면에 표시된 이후 Fade In 연출로 스테이지가 시작됩니다. */
	UFUNCTION(BlueprintCallable)
	void StartStage();

	/** Fade Out 연출로 스테이지가 종료됩니다. */
	UFUNCTION(BlueprintCallable)
	void EndStage();

protected:
	UPROPERTY()
	TSubclassOf<UChapterWidget> ChapterWidgetClass;

	UPROPERTY()
	UChapterWidget* ChapterWidgetInstance;

private:
	/** 현재 스테이지를 나타내는 변수입니다. */
	uint8 CurrentStageIndex = 0;

public:
	FORCEINLINE UChapterWidget* GetChapterWidget() const { return ChapterWidgetInstance; }
	
};
