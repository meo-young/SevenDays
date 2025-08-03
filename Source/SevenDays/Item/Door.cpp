#include "Item/Door.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "SevenDays.h"
#include "Character/SDCharacter.h"
#include "Components/BoxComponent.h"
#include "Define/DefineClass.h"
#include "Subsystem/StageSubsystem.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetupAttachment(RootComponent);
	InteractionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionBox->SetCollisionResponseToChannel(ECC_DOOR, ECR_Block);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	ALevelSequenceActor* OutActor;
	DoorOpenLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), DoorOpenSequence, FMovieSceneSequencePlaybackSettings(), OutActor);
	DoorLockedLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), DoorLockedSequence, FMovieSceneSequencePlaybackSettings(), OutActor);
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UStageSubsystem* StageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UStageSubsystem>();
	if (StageSubsystem->IsStageSucceeded())
	{
		bIsCanOpen = true;
	}
	else
	{
		bIsCanOpen = false;
	}
}

void ADoor::Interact_Implementation()
{
	IInteractable::Interact_Implementation();

	if (bIsCanOpen && DoorOpenSequence)
	{
		if (!DoorOpenLevelSequencePlayer->IsPlaying())
		{
			DoorOpenLevelSequencePlayer->Play();
		}

		ASDCharacter* Player = Cast<ASDCharacter>(GetGameInstance()->GetFirstLocalPlayerController()->GetPawn());
		Player->DisablePlayerInput();

		FTimerHandle StageEndHandle;
		GetWorld()->GetTimerManager().SetTimer(StageEndHandle, [this]()
		{
			UStageSubsystem* StageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UStageSubsystem>();
			StageSubsystem->EndStage();
		}, 2.0f, false);
	}
	else
	{
		if (!DoorLockedLevelSequencePlayer->IsPlaying())
		{
			DoorLockedLevelSequencePlayer->Play();
		}
	}
}
