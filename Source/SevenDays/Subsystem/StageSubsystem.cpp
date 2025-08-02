#include "Subsystem/StageSubsystem.h"
#include "EngineUtils.h"
#include "SevenDays.h"
#include "Define/DefineClass.h"
#include "Character/SDCharacter.h"
#include "LevelSequence/MissingLevelSequenceActor.h"
#include "LevelSequence/NewLevelSequenceActor.h"
#include "DataTableRow/StageEventTableRow.h"
#include "GameFramework/PlayerStart.h"
#include "UI/ChapterWidget.h"
#include "UI/MissionWidget.h"
#include "UI/FadeWidget.h"

UStageSubsystem::UStageSubsystem()
{
	static ConstructorHelpers::FClassFinder<UChapterWidget> WBP_Chapter(TEXT("/Game/_SevenDays/HUD/WBP_Chapter"));
	if (WBP_Chapter.Succeeded())
	{
		ChapterWidgetClass = WBP_Chapter.Class;
	}

	static ConstructorHelpers::FClassFinder<UMissionWidget> WBP_Mission(TEXT("/Game/_SevenDays/HUD/WBP_Mission"));
	if (WBP_Mission.Succeeded())
	{
		MissionWidgetClass = WBP_Mission.Class;
	}

	static ConstructorHelpers::FClassFinder<UFadeWidget> WBP_Fade(TEXT("/Game/_SevenDays/HUD/WBP_Fade"));
	if (WBP_Fade.Succeeded())
	{
		FadeWidgetClass = WBP_Fade.Class;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_StageEvent(TEXT("/Game/_SevenDays/DataTable/DT_StageEvent"));
	if (DT_StageEvent.Succeeded())
	{
		StageEventDataTable = DT_StageEvent.Object;
	}
}

void UStageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

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
	// 1. Fade 위젯을 제거합니다.
	if (FadeWidgetInstance && FadeWidgetInstance->IsInViewport())
	{
		FadeWidgetInstance->RemoveFromParent();
	}
	
	// 2. 플레이어 입력을 비활성화 합니다.
	ASDCharacter* Player = Cast<ASDCharacter>(GetGameInstance()->GetFirstLocalPlayerController()->GetPawn());
	Player->DisablePlayerInput();
	
	// 3. EyeCatch를 화면에 표시합니다.
	ChapterWidgetInstance->ShowEyeCatch(CurrentStageIndex);

	// 4. 현재 스테이지에 대한 이벤트 정보를 가져옵니다.
	GetCurrentStageEvent();

	// 5. 현재 스테이지에 대한 첫 번째 이벤트를 출력합니다.
	ShowCurrentStageEvent();
}

void UStageSubsystem::EndStage()
{
	// 1. 스테이지를 한 단계 증가시킵니다.
	CurrentStageIndex++;
	
	// 2. Fade Out 연출을 시작합니다.
	FadeWidgetInstance->FadeOut();

	// 3. 플레이어를 원래 위치로 복귀시킵니다.
	FTimerHandle TeleportHandle;
	GetWorld()->GetTimerManager().SetTimer(TeleportHandle, this, &ThisClass::TeleportPlayerToStartPoint, 3.0f, false);

	// 4. 다음 스테이지를 시작합니다.
	FTimerHandle StartStageHandle;
	GetWorld()->GetTimerManager().SetTimer(StartStageHandle, this, &ThisClass::StartStage, 3.0f, false);
}

void UStageSubsystem::GetCurrentStageEvent()
{
	// 현재 스테이지에 대한 이벤트를 가져옵니다.
	const FStageEventTableRow* CurrentStageEvent = StageEventRows[CurrentStageIndex];
	CurrentStageMissionTypes = CurrentStageEvent->MissionTypes;
}

void UStageSubsystem::ShowCurrentStageEvent()
{
	// 현재 스테이지에 대한 이벤트를 모두 출력한 경우 종료합니다.
	if (CurrentStageMissionTypes.Num() <= 0)
	{
		EndStage();
		return;
	}

	// 이벤트 타입에 따른 UI를 출력합니다.
	MissionWidgetInstance->ShowMissionWidget(CurrentStageMissionTypes[0]);

	// 이벤트 타입에 따라 이벤트를 출력합니다.
	uint8 RandIndex = 0;
	ASDLevelSequenceActor* CurrentEvent = nullptr;
	switch (CurrentStageMissionTypes[0])
	{
		case EMissionType::Missing:
			RandIndex = FMath::RandRange(0, MissingEvents.Num() - 1);
			CurrentEvent = MissingEvents[RandIndex];
			MissingEvents.RemoveAt(RandIndex);
			break;
		case EMissionType::New:
			RandIndex = FMath::RandRange(0, NewEvents.Num() - 1);
			CurrentEvent = NewEvents[RandIndex];
			NewEvents.RemoveAt(RandIndex);
			break;
		case EMissionType::Horror:
			break;
	}

	// 현재 이벤트에 대해 상호작용이 끝났을 때 호출되는 델리게이트에 ShowCurrentStageEvent를 바인딩합니다.
	if (CurrentEvent)
	{
		CurrentEvent->PlayLevelSequenceForLoop();
		CurrentEvent->OnInteractDelegate.AddDynamic(this, &ThisClass::ShowCurrentStageEvent);
	}

	// 출력한 후 해당 이벤트는 배열에서 제외합니다.
	CurrentStageMissionTypes.RemoveAt(0);
}

void UStageSubsystem::TeleportPlayerToStartPoint()
{
	for (APlayerStart* PlayerStart : TActorRange<APlayerStart>(GetWorld()))
	{
		if (PlayerStart)
		{
			if (ASDCharacter* Player = Cast<ASDCharacter>(GetGameInstance()->GetFirstLocalPlayerController()->GetPawn()))
			{
				Player->SetActorLocation(PlayerStart->GetActorLocation());
				Player->GetController()->SetControlRotation(PlayerStart->GetActorRotation());
				break;
			}
		}
	}
}
