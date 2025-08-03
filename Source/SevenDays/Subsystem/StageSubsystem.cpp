#include "Subsystem/StageSubsystem.h"
#include "EngineUtils.h"
#include "LevelSequencePlayer.h"
#include "SevenDays.h"
#include "SoundSubsystem.h"
#include "Define/DefineClass.h"
#include "Character/SDCharacter.h"
#include "LevelSequence/MissingLevelSequenceActor.h"
#include "LevelSequence/NewLevelSequenceActor.h"
#include "DataTableRow/StageEventTableRow.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "LevelSequence/HorrorLevelSequenceActor.h"
#include "UI/ChapterWidget.h"
#include "UI/MissionWidget.h"
#include "UI/FadeWidget.h"

void UStageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	StageEventDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/_SevenDays/DataTable/DT_StageEvent"));
	ChapterWidgetClass = LoadClass<UChapterWidget>(nullptr, TEXT("/Game/_SevenDays/HUD/WBP_Chapter.WBP_Chapter_C"));
	MissionWidgetClass = LoadClass<UMissionWidget>(nullptr, TEXT("/Game/_SevenDays/HUD/WBP_Mission.WBP_Mission_C"));
	FadeWidgetClass = LoadClass<UFadeWidget>(nullptr, TEXT("/Game/_SevenDays/HUD/WBP_Fade.WBP_Fade_C"));
	EventLevelSequence = LoadObject<ULevelSequence>(nullptr, TEXT("/Game/_SevenDays/LevelSequence/LS_ShowEvent.LS_ShowEvent"));

	if (ChapterWidgetClass)
	{
		ChapterWidgetInstance = CreateWidget<UChapterWidget>(GetGameInstance()->GetWorld(), ChapterWidgetClass);
	}

	if (MissionWidgetClass)
	{
		MissionWidgetInstance = CreateWidget<UMissionWidget>(GetGameInstance()->GetWorld(), MissionWidgetClass);
	}

	if (FadeWidgetClass)
	{
		FadeWidgetInstance = CreateWidget<UFadeWidget>(GetGameInstance()->GetWorld(), FadeWidgetClass);
	}

	if (StageEventDataTable)
	{
		StageEventDataTable->GetAllRows(TEXT(""), StageEventRows);
	}

	CurrentStageIndex = 1;
}

void UStageSubsystem::StartStage()
{
	// 1. 이벤트 인스턴스를 초기화합니다.
	if (bIsFirstTime)
	{
		MissingEventInstances = MissingEvents;
		NewEventInstances = NewEvents;
		HorrorEventInstances = HorrorEvents; 
		bIsFirstTime = false;
	}
	
	// 2. Fade 위젯을 제거합니다.
	if (FadeWidgetInstance && FadeWidgetInstance->IsInViewport())
	{
		FadeWidgetInstance->RemoveFromParent();
	}

	// 3. 현재 스테이지에 맞는 분위기 LevelSequence를 재생합니다.
	ULevelSequence* CurrentStageLevelSequence = StageEventRows[CurrentStageIndex]->StageLevelSequence;
	ALevelSequenceActor* OutActor;
	ULevelSequencePlayer* LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), CurrentStageLevelSequence, FMovieSceneSequencePlaybackSettings(), OutActor);
	LevelSequencePlayer->Play();
	
	// 4. 플레이어 입력을 비활성화 합니다.
	ASDCharacter* Player = Cast<ASDCharacter>(GetGameInstance()->GetFirstLocalPlayerController()->GetPawn());
	Player->DisablePlayerInput();
	
	// 5. EyeCatch를 화면에 표시합니다.
	if (ChapterWidgetInstance)
	{
		ChapterWidgetInstance->ShowEyeCatch(CurrentStageIndex);
	}
}

void UStageSubsystem::EndStage()
{
	// 스테이지를 한 단계 증가시킵니다.
	CurrentStageIndex++;
	
	CleanStage();
}

void UStageSubsystem::InitStage()
{
	// 스테이지를 1로 초기화합니다.
	CurrentStageIndex = 1;
	
	CleanStage();
}

void UStageSubsystem::ShowEvent()
{
	// 현재 스테이지에 대한 이벤트 정보를 가져옵니다.
	GetCurrentStageEvent();

	// 이벤트를 출력합니다.
	CheckCurrentStageEvent();
}

void UStageSubsystem::GetCurrentStageEvent()
{
	// 현재 스테이지에 대한 이벤트를 가져옵니다.
	const FStageEventTableRow* CurrentStageEvent = StageEventRows[CurrentStageIndex];
	CurrentStageMissionTypes = CurrentStageEvent->MissionTypes;
}

void UStageSubsystem::CheckCurrentStageEvent()
{
	// 현재 스테이지에 대한 이벤트를 모두 출력한 경우 종료합니다.
	if (CurrentStageMissionTypes.Num() <= 0)
	{
		USoundSubsystem* SoundSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USoundSubsystem>();
		SoundSubsystem->PlaySFX(ESFX::UnLockedDoor, FVector(538.0f, -250.0f, 105.0f));
		
		SetStageStarted(false);
		SetStageSucceeded(true);
		return;
	}

	// 현재 스테이지에 대한 이벤트가 출력되었음을 알리는 LevelSequence를 재생합니다.
	ALevelSequenceActor* OutActor = nullptr;
	ULevelSequencePlayer* LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), EventLevelSequence, FMovieSceneSequencePlaybackSettings(), OutActor);
	LevelSequencePlayer->Play();

	// 0.5초 후 이벤트를 출력합니다.
	FTimerHandle ShowEventHandle;
	GetWorld()->GetTimerManager().SetTimer(ShowEventHandle, this, &ThisClass::ShowStageEvent, 0.5f, false);
	
}

void UStageSubsystem::ShowStageEvent()
{
	// 이벤트 타입에 따른 UI를 출력합니다.
	MissionWidgetInstance->ShowMissionWidget(CurrentStageMissionTypes[0]);

	// 이벤트 타입에 따라 이벤트를 출력합니다.
	// 모든 이벤트를 사용한 경우 다시 초기화합니다.
	uint8 RandIndex = 0;
	CurrentEvent = nullptr;
	switch (CurrentStageMissionTypes[0])
	{
	case EMissionType::Missing:
		RandIndex = FMath::RandRange(0, MissingEventInstances.Num() - 1);
		CurrentEvent = MissingEventInstances[RandIndex];
		MissingEventInstances.RemoveAt(RandIndex);
		if (MissingEventInstances.Num() <= 0)
		{
			MissingEventInstances = MissingEvents; 
		}
		break;
		
	case EMissionType::New:
		RandIndex = FMath::RandRange(0, NewEventInstances.Num() - 1);
		CurrentEvent = NewEventInstances[RandIndex];
		NewEventInstances.RemoveAt(RandIndex);
		if (NewEventInstances.Num() <= 0)
		{
			NewEventInstances = NewEvents; 
		}
		break;
		
	case EMissionType::Horror:
		RandIndex = FMath::RandRange(0, HorrorEventInstances.Num() - 1);
		CurrentEvent = HorrorEventInstances[RandIndex];
		HorrorEventInstances.RemoveAt(RandIndex);
		if (HorrorEventInstances.Num() <= 0)
		{
			HorrorEventInstances = HorrorEvents; 
		}
		break;
	}

	// 현재 이벤트에 대해 상호작용이 끝났을 때 호출되는 델리게이트에 ShowCurrentStageEvent를 바인딩합니다.
	if (CurrentEvent)
	{
		CurrentEvent->PlayLevelSequenceForLoop();
		CurrentEvent->OnInteractDelegate.AddDynamic(this, &ThisClass::CallShowCurrentStageEventWithDelay);
	}

	// 출력한 후 해당 이벤트는 배열에서 제외합니다.
	CurrentStageMissionTypes.RemoveAt(0);
}

void UStageSubsystem::CleanStage()
{
	// 1. 현재 이벤트가 재생 중인 경우 중지합니다.
	if (CurrentEvent && CurrentEvent->GetLevelSequencePlayer()->IsPlaying())
	{
		CurrentEvent->StopLevelSequence();
	}
	
	// 2. Fade Out 연출을 시작합니다.
	FadeWidgetInstance->FadeOut();

	// 3. 다음 스테이지를 시작합니다.
	FTimerHandle StartStageHandle;
	GetWorld()->GetTimerManager().SetTimer(StartStageHandle, this, &ThisClass::StartStage, 3.0f, false);

	// 4. 플레이어를 다시 생성합니다.
	FTimerHandle RespawnHandle;
	GetWorld()->GetTimerManager().SetTimer(RespawnHandle, FTimerDelegate::CreateLambda([this]()
	{
		GetWorld()->GetFirstPlayerController()->GetPawn()->Destroy();
		GetWorld()->GetAuthGameMode()->RestartPlayer(GetGameInstance()->GetFirstLocalPlayerController());

		ASDCharacter* Player = Cast<ASDCharacter>(GetGameInstance()->GetFirstLocalPlayerController()->GetPawn());
		Player->DisablePlayerInput();
	}), 3.5f, false);
}

void UStageSubsystem::CallShowCurrentStageEventWithDelay()
{
	FTimerHandle ShowEventHandle;
	GetWorld()->GetTimerManager().SetTimer(ShowEventHandle, this, &ThisClass::CheckCurrentStageEvent, 3.0f, false);
}
