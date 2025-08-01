#include "Character/SDPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "SevenDays.h"

void ASDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem =  GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		if (MappingContext)
		{
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);
		}
	}
}
