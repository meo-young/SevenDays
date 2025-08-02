#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"
#include "BGMDataTableRow.generated.h"

enum class EBGM : uint8;

USTRUCT(BlueprintType)
struct FBGMDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	EBGM BGMType;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundCue> SoundCue;
	
};
