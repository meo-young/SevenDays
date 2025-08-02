#pragma once

#include "CoreMinimal.h"
#include "LevelSequence/SDLevelSequenceActor.h"
#include "HorrorLevelSequenceActor.generated.h"

UCLASS()
class SEVENDAYS_API AHorrorLevelSequenceActor : public ASDLevelSequenceActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
};
