#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SDPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class SEVENDAYS_API ASDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* MappingContext;
	
};
