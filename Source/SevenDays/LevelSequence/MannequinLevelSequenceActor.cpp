#include "LevelSequence/MannequinLevelSequenceActor.h"
#include "Subsystem/StageSubsystem.h"

void AMannequinLevelSequenceActor::BeginPlay()
{
	Super::BeginPlay();

	UStageSubsystem* StageSubsystem = GetGameInstance()->GetSubsystem<UStageSubsystem>();
	StageSubsystem->AddMannequinEvent(this);
}
