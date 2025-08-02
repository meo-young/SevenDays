#pragma once

#include "CoreMinimal.h"
#include "LevelSequence/SDLevelSequenceActor.h"
#include "MissingLevelSequenceActor.generated.h"

UCLASS()
class SEVENDAYS_API AMissingLevelSequenceActor : public ASDLevelSequenceActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
};
