#pragma once

#include "CoreMinimal.h"
#include "LevelSequence/SDLevelSequenceActor.h"
#include "NewLevelSequenceActor.generated.h"

UCLASS()
class SEVENDAYS_API ANewLevelSequenceActor : public ASDLevelSequenceActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
};
