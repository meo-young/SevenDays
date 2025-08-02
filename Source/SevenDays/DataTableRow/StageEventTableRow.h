#pragma once

#include "CoreMinimal.h"
#include "StageEventTableRow.generated.h"

enum class EMissionType : uint8;

USTRUCT()
struct FStageEventTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<EMissionType> MissionTypes;
	
};