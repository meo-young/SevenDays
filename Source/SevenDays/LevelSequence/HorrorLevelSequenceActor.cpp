#include "LevelSequence/HorrorLevelSequenceActor.h"
#include "SevenDays.h"
#include "Subsystem/StageSubsystem.h"

void AHorrorLevelSequenceActor::BeginPlay()
{
	Super::BeginPlay();

	UStageSubsystem* StageSubsystem = GetGameInstance()->GetSubsystem<UStageSubsystem>();
	StageSubsystem->AddHorrorEvent(this);

	LOG(TEXT("Missing Level Sequence Actor BeginPlay: %s"), *GetName());
}
