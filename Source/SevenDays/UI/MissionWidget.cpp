#include "UI/MissionWidget.h"
#include "EngineUtils.h"
#include "SevenDays.h"
#include "Tutorial/TutorialManager.h"

void UMissionWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	LOG(TEXT("미션 위젯 생성됨"));

	for (ATutorialManager* TutorialManager : TActorRange<ATutorialManager>(GetWorld()))
	{
		if (IsValid(TutorialManager))
		{
			LOG(TEXT("튜토리얼 매니저 바인딩"));
			TutorialManager->WASDTutorial.TutorialStartDelegate.AddUObject(this, &ThisClass::ShowWASDTutorialWidget);
			TutorialManager->WASDTutorial.TutorialEndDelegate.AddUObject(this, &ThisClass::RemoveWASDTutorialWidget);
			
			TutorialManager->PictureTutorial.TutorialStartDelegate.AddUObject(this, &ThisClass::ShowPictureTutorialWidget);
			TutorialManager->PictureTutorial.TutorialEndDelegate.AddUObject(this, &ThisClass::RemovePictureTutorialWidget);

			TutorialManager->DoorOpenTutorial.TutorialStartDelegate.AddUObject(this, &ThisClass::ShowDoorOpenTutorialWidget);
			TutorialManager->DoorOpenTutorial.TutorialEndDelegate.AddUObject(this, &ThisClass::RemoveDoorOpenTutorialWidget);
		}
	}
}
