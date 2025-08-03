#include "LevelSequence/NewLevelSequenceActor.h"
#include "SevenDays.h"
#include "Subsystem/StageSubsystem.h"

void ANewLevelSequenceActor::BeginPlay()
{
	Super::BeginPlay();

	UStageSubsystem* StageSubsystem = GetGameInstance()->GetSubsystem<UStageSubsystem>();
	StageSubsystem->AddNewEvent(this);
}
