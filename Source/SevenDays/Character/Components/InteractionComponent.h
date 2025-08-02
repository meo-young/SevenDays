#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class IInteractable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SEVENDAYS_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void StartInteraction();

private:
	FHitResult InteractionHitResult;
	FCollisionQueryParams QueryParams;
	IInteractable* InteractableObject = nullptr;
	
};
