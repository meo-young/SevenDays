#include "Tutorial/TutorialManager.h"
#include "SevenDays.h"
#include "Character/SDCharacter.h"
#include "Subsystem/StageSubsystem.h"
#include "UI/MissionWidget.h"

ATutorialManager::ATutorialManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATutorialManager::BeginPlay()
{
	Super::BeginPlay();

	UStageSubsystem* StageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UStageSubsystem>();
	StageSubsystem->GetMissionWidget()->AddToViewport(100);

	WASDTutorial.TutorialStartDelegate.AddLambda([this](){
		ASDCharacter* Player = Cast<ASDCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		Player->SetEnableMove(true);
	});

	PictureTutorial.TutorialStartDelegate.AddLambda([this]()
	{
		ASDCharacter* Player = Cast<ASDCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		Player->SetEnableInteract(true);
	});

	DoorOpenTutorial.TutorialStartDelegate.AddLambda([this]()
	{
		ASDCharacter* Player = Cast<ASDCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		Player->SetEnableDoorOpen(true);
	});
	
	WASDTutorial.TutorialCondition.BindUObject(this, &ThisClass::WASDTutorialCondition);
	PictureTutorial.TutorialCondition.BindUObject(this, &ThisClass::PictureTutorialCondition);
	DoorOpenTutorial.TutorialCondition.BindUObject(this, &ThisClass::DoorOpenTutorialCondition);

	TutorialEventList.Empty();
	TutorialEventList.Emplace(WASDTutorial);
	TutorialEventList.Emplace(PictureTutorial);
	TutorialEventList.Emplace(DoorOpenTutorial);
	
}

void ATutorialManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsTutorialActive)
	{
		if (TutorialEventList[0].TutorialCondition.Execute())
		{
			NextTutorial();
		}
	}
}

void ATutorialManager::StartTutorial()
{
	ASDCharacter* Player = Cast<ASDCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	Player->SetBatteryActive(false);
	
	if (!TutorialEventList.IsEmpty())
	{
		if (TutorialEventList[0].TutorialStartDelegate.IsBound())
		{
			bIsTutorialActive = true;
			TutorialEventList[0].TutorialStartDelegate.Broadcast();
		}
	}
	else
	{
		if (OnAllTutorialEndDelegate.IsBound())
		{
			OnAllTutorialEndDelegate.Broadcast();
		}
		
		Player->SetBatteryActive(true);
		
		SetActorTickEnabled(false);
	}
}

void ATutorialManager::NextTutorial()
{
	bIsTutorialActive = false;
	
	GetWorldTimerManager().SetTimer(TutorialHandle, this, &ThisClass::RemoveTutorial, 1.0f, false);
}

void ATutorialManager::RemoveTutorial()
{
	if (TutorialEventList[0].TutorialEndDelegate.IsBound())
	{
		TutorialEventList[0].TutorialEndDelegate.Broadcast();
	}

	TutorialEventList.RemoveAt(0);

	GetWorldTimerManager().SetTimer(TutorialHandle, this, &ThisClass::StartTutorial, 3.0f, false);
}

bool ATutorialManager::WASDTutorialCondition()
{
	const bool bWDown = GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::W);
	const bool bADown = GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::A);
	const bool bSDown = GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::S);
	const bool bDDown = GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::D);

	return bWDown || bADown || bSDown || bDDown; 
}

bool ATutorialManager::PictureTutorialCondition()
{
	return bPictureInputDown;
}

bool ATutorialManager::DoorOpenTutorialCondition()
{
	return bDoorOpenInputDown;
}
