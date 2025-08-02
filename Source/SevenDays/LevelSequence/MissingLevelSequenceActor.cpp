#include "LevelSequence/MissingLevelSequenceActor.h"
#include "SevenDays.h"
#include "Subsystem/StageSubsystem.h"

void AMissingLevelSequenceActor::BeginPlay()
{
	Super::BeginPlay();

	UStageSubsystem* StageSubsystem = GetGameInstance()->GetSubsystem<UStageSubsystem>();
	StageSubsystem->AddMissingEvent(this);

	LOG(TEXT("Missing Level Sequence Actor BeginPlay: %s"), *GetName());
}
