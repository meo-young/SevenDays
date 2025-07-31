#include "Character/SDPlayerController.h"
#include "EnhancedInputSubsystems.h"

void ASDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem =  ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (MappingContext)
		{
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputLocalPlayerSubsystem이 유효하지 않습니다"));
	}
}
