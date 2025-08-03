#pragma once

#include "CoreMinimal.h"
#include "LevelSequence/SDLevelSequenceActor.h"
#include "MannequinLevelSequenceActor.generated.h"

UCLASS()
class SEVENDAYS_API AMannequinLevelSequenceActor : public ASDLevelSequenceActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
};
