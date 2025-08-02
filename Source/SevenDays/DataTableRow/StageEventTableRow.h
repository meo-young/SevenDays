#pragma once

#include "CoreMinimal.h"
#include "StageEventTableRow.generated.h"

class ULevelSequence;
enum class EMissionType : uint8;

USTRUCT()
struct FStageEventTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<EMissionType> MissionTypes;

	UPROPERTY(EditAnywhere)
	ULevelSequence* StageLevelSequence;
	
};