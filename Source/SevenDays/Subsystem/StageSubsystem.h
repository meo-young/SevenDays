#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StageSubsystem.generated.h"

class ASDLevelSequenceActor;
class AHorrorLevelSequenceActor;
class UFadeWidget;
struct FStageEventTableRow;
class ANewLevelSequenceActor;
class AMissingLevelSequenceActor;
enum class EMissionType : uint8;
class UMissionWidget;
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

	/** 스테이지를 처음부터 시작하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void InitStage();

private:
	/** 현재 스테이지에 대한 이벤트 정보를 가져옵니다. */
	void GetCurrentStageEvent();

	/** 현재 스테이지에 대한 첫 번째 이벤트를 출력합니다. */
	UFUNCTION()
	void ShowCurrentStageEvent();

	/** 플레이어를 시작 지점으로 텔레포트하는 함수입니다. */
	UFUNCTION()
	void TeleportPlayerToStartPoint();

	/** 스테이지를 정리하는 함수입니다. */
	void CleanStage();

protected:
	/** Chapter Widget 클래스입니다. */
	UPROPERTY()
	TSubclassOf<UChapterWidget> ChapterWidgetClass;

	/** Chapter Widget 인스턴스입니다. */
	UPROPERTY()
	UChapterWidget* ChapterWidgetInstance = nullptr;

	/** Mission Widget 클래스입니다. */
	UPROPERTY()
	TSubclassOf<UMissionWidget> MissionWidgetClass;

	/** Mission Widget 인스턴스입니다. */
	UPROPERTY()
	UMissionWidget* MissionWidgetInstance = nullptr;

	/** Fade Widget 클래스입니다. */
	UPROPERTY()
	TSubclassOf<UFadeWidget> FadeWidgetClass;

	/** Fade Widget 인스턴스입니다. */
	UPROPERTY()
	UFadeWidget* FadeWidgetInstance = nullptr;

	/** 맵에 배치되어 있는 Missing Event에 대한 참조 변수입니다. */
	UPROPERTY()
	TArray<AMissingLevelSequenceActor*> MissingEvents;

	/** 맵에 배치되어 있는 New Event에 대한 참조 변수입니다. */
	UPROPERTY()
	TArray<ANewLevelSequenceActor*> NewEvents;

	/** 맵에 배치되어 있는 Horror Event에 대한 참조 변수입니다. */
	UPROPERTY()
	TArray<AHorrorLevelSequenceActor*> HorrorEvents;

	/** 스테이지 이벤트 데이터 테이블입니다. */
	UPROPERTY()
	UDataTable* StageEventDataTable = nullptr;

	/** 스테이지 이벤트 데이터 테이블의 모든 행을 저장하는 변수입니다. */
	TArray<FStageEventTableRow*> StageEventRows;

	/** 현재 스테이지의 미션 타입을 저장하는 변수입니다. */
	TArray<EMissionType> CurrentStageMissionTypes;

private:
	/** 현재 스테이지를 나타내는 변수입니다. */
	uint8 CurrentStageIndex = 0;

	/** Missing Event에 대한 인스턴스 변수입니다. */
	UPROPERTY()
	TArray<AMissingLevelSequenceActor*> MissingEventInstances;

	/** New Event에 대한 인스턴스 변수입니다. */
	UPROPERTY()
	TArray<ANewLevelSequenceActor*> NewEventInstances;

	/** Horror Event에 대한 인스턴스 변수입니다. */
	UPROPERTY()
	TArray<AHorrorLevelSequenceActor*> HorrorEventInstances;

	/** 인스턴스 초기화를 위한 변수입니다. */
	uint8 bIsFirstTime : 1 = true;

	/** 현재 스테이지 이벤트에 대한 참조 변수입니다. */
	UPROPERTY()
	ASDLevelSequenceActor* CurrentEvent = nullptr;

public:
	FORCEINLINE UChapterWidget* GetChapterWidget() const { return ChapterWidgetInstance; }
	FORCEINLINE UMissionWidget* GetMissionWidget() const { return MissionWidgetInstance; }
	FORCEINLINE void AddMissingEvent(AMissingLevelSequenceActor* MissingEvent) { MissingEvents.Emplace(MissingEvent); }
	FORCEINLINE void AddNewEvent(ANewLevelSequenceActor* NewEvent) { NewEvents.Emplace(NewEvent); }
	FORCEINLINE void AddHorrorEvent(AHorrorLevelSequenceActor* HorrorEvent) { HorrorEvents.Emplace(HorrorEvent); }
	
};
