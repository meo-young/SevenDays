#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StageSubsystem.generated.h"

class AMannequinLevelSequenceActor;
class ASDLevelSequenceActor;
class AHorrorLevelSequenceActor;
class UFadeWidget;
struct FStageEventTableRow;
class ANewLevelSequenceActor;
class AMissingLevelSequenceActor;
enum class EMissionType : uint8;
class UMissionWidget;
class UChapterWidget;
class ULevelSequence;

UCLASS(Blueprintable)
class SEVENDAYS_API UStageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
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

	/** 현재 스테이지에 대한 이벤트를 출력하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void ShowEvent();

	/** 문이 열리는 이벤트입니다. */
	UFUNCTION(BlueprintCallable)
	void OpenDoorEvent();

private:
	/** 현재 스테이지에 대한 이벤트 정보를 가져옵니다. */
	void GetCurrentStageEvent();

	/** 현재 스테이지에 대한 이벤트를 확인합니다. */
	UFUNCTION()
	void CheckCurrentStageEvent();

	/** 현재 스테이지 이벤트를 출력하는 함수입니다. */
	void ShowStageEvent();

	/** 스테이지를 정리하는 함수입니다. */
	void CleanStage();

	/** 현재 스테이지 이벤트를 지연 시간 후에 호출하는 함수입니다. */
	UFUNCTION()
	void CallShowCurrentStageEventWithDelay();

protected:
	/** Chapter Widget 클래스입니다. */
	UPROPERTY()
	TSubclassOf<UChapterWidget> ChapterWidgetClass;

	/** Chapter Widget 인스턴스입니다. */
	UPROPERTY()
	TObjectPtr<UChapterWidget> ChapterWidgetInstance = nullptr;

	/** Mission Widget 클래스입니다. */
	UPROPERTY()
	TSubclassOf<UMissionWidget> MissionWidgetClass;

	/** Mission Widget 인스턴스입니다. */
	UPROPERTY()
	TObjectPtr<UMissionWidget> MissionWidgetInstance = nullptr;

	/** Fade Widget 클래스입니다. */
	UPROPERTY()
	TSubclassOf<UFadeWidget> FadeWidgetClass;

	/** Fade Widget 인스턴스입니다. */
	UPROPERTY()
	TObjectPtr<UFadeWidget> FadeWidgetInstance = nullptr;

	/** 맵에 배치되어 있는 Missing Event에 대한 참조 변수입니다. */
	UPROPERTY()
	TArray<AMissingLevelSequenceActor*> MissingEvents;

	/** 맵에 배치되어 있는 New Event에 대한 참조 변수입니다. */
	UPROPERTY()
	TArray<ANewLevelSequenceActor*> NewEvents;

	/** 맵에 배치되어 있는 Horror Event에 대한 참조 변수입니다. */
	UPROPERTY()
	TArray<AHorrorLevelSequenceActor*> HorrorEvents;

	/** 맵에 배치되어 있는 Mannequin Event에 대한 참조 변수입니다. */
	UPROPERTY()
	TArray<AMannequinLevelSequenceActor*> MannequinEvents;

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

	/** Mannequin Event에 대한 인스턴스 변수입니다. */
	UPROPERTY()
	TArray<AMannequinLevelSequenceActor*> MannequinEventInstances;

	/** 인스턴스 초기화를 위한 변수입니다. */
	uint8 bIsFirstTime : 1 = true;

	/** 현재 스테이지 이벤트에 대한 참조 변수입니다. */
	UPROPERTY()
	ASDLevelSequenceActor* CurrentEvent = nullptr;

	/** 현재 스테이지가 시작되었는지 여부를 나타내는 변수입니다. */
	UPROPERTY()
	uint8 bIsStageStarted : 1 = false;

	/** 현재 스테이지가 성공적으로 완료되었는지 여부를 나타내는 변수입니다. */
	UPROPERTY()
	uint8 bIsStageSucceeded : 1 = false;

	/** 이벤트가 출력될 때 사용할 LevelSequence 입니다. */
	UPROPERTY()
	TObjectPtr<ULevelSequence> EventLevelSequence = nullptr;

public:
	FORCEINLINE UChapterWidget* GetChapterWidget() const { return ChapterWidgetInstance; }
	FORCEINLINE UMissionWidget* GetMissionWidget() const { return MissionWidgetInstance; }
	FORCEINLINE void AddMissingEvent(AMissingLevelSequenceActor* MissingEvent) { MissingEvents.Emplace(MissingEvent); }
	FORCEINLINE void AddNewEvent(ANewLevelSequenceActor* NewEvent) { NewEvents.Emplace(NewEvent); }
	FORCEINLINE void AddHorrorEvent(AHorrorLevelSequenceActor* HorrorEvent) { HorrorEvents.Emplace(HorrorEvent); }
	FORCEINLINE void AddMannequinEvent(AMannequinLevelSequenceActor* MannequinEvent) { MannequinEvents.Emplace(MannequinEvent); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE uint8 GetCurrentStageIndex() const { return CurrentStageIndex; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCurrentStageIndex(const uint8 StageIndex) { CurrentStageIndex = StageIndex; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsStageStarted() const { return bIsStageStarted; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetStageStarted(bool bStarted) { bIsStageStarted = bStarted; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsStageSucceeded() const { return bIsStageSucceeded; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetStageSucceeded(bool bSucceeded) { bIsStageSucceeded = bSucceeded; }
	
};
