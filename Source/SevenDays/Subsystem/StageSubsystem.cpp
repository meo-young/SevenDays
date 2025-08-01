#include "Subsystem/StageSubsystem.h"
#include "SevenDays.h"
#include "Define/DefineClass.h"
#include "Character/SDCharacter.h"
#include "UI/ChapterWidget.h"
#include "UI/MissionWidget.h"

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
}

void UStageSubsystem::StartStage()
{
	// 1. 플레이어 입력을 비활성화 합니다.
	ASDCharacter* Player = Cast<ASDCharacter>(GetGameInstance()->GetFirstLocalPlayerController()->GetPawn());
	Player->DisablePlayerInput();
	
	// 2. EyeCatch를 화면에 표시합니다.
	ChapterWidgetInstance->ShowEyeCatch(CurrentStageIndex);

	// 3. 스테이지 시작 후 필요한 초기화 작업을 수행합니다.
	EMissionType MissionTypes = EMissionType::Missing;

	MissionWidgetInstance->AddToViewport(100);
	MissionWidgetInstance->ShowMissionWidget(MissionTypes);
}

void UStageSubsystem::EndStage()
{
	// 1. Fade Out 연출을 시작합니다.

	// 2. Stage를 원래 상태로 복구합니다.
}
