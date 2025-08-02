#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"
#include "SFXDataTableRow.generated.h"

enum class ESFX : uint8;

USTRUCT(BlueprintType)
struct FSFXDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	ESFX SFXType;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundCue> SoundCue;
};

