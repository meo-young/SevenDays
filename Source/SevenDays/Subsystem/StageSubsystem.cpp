#include "Subsystem/StageSubsystem.h"
#include "SevenDays.h"
#include "Character/SDCharacter.h"
#include "UI/ChapterWidget.h"

UStageSubsystem::UStageSubsystem()
{
	static ConstructorHelpers::FClassFinder<UChapterWidget> WBP_Chapter(TEXT("/Game/_SevenDays/HUD/WBP_Chapter"));
	if (WBP_Chapter.Succeeded())
	{
		ChapterWidgetClass = WBP_Chapter.Class;
	}
}

void UStageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (ChapterWidgetClass)
	{
		ChapterWidgetInstance = CreateWidget<UChapterWidget>(GetGameInstance()->GetWorld(), ChapterWidgetClass);
	}
}

void UStageSubsystem::StartStage()
{
	// 1. EyeCatch를 화면에 표시합니다.
	ChapterWidgetInstance->ShowEyeCatch(CurrentStageIndex);

	// 2. Fade In 연출을 시작합니다.

	// 3. 스테이지 시작 후 필요한 초기화 작업을 수행합니다.
}

void UStageSubsystem::EndStage()
{
	// 1. Fade Out 연출을 시작합니다.

	// 2. Stage를 원래 상태로 복구합니다.
}
