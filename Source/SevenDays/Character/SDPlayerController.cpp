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
			LOG(TEXT("MappingContext 바인드"));
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);
		}
	}
}
