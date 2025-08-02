#include "Character/Components/InteractionComponent.h"
#include "SevenDays.h"
#include "Camera/CameraComponent.h"
#include "Define/DefineClass.h"
#include "Interface/Interactable.h"

void UInteractionComponent::StartInteraction()
{
	// 카메라의 현재 위치에서 정면을 기준으로 LineTracing을 수행합니다.
	const UCameraComponent* CameraComponent = GetOwner()->FindComponentByClass<UCameraComponent>();
	const FVector CurrentLocation = CameraComponent->GetComponentLocation();
	const FVector ForwardVector = CameraComponent->GetForwardVector();
	const FVector TargetLocation = CurrentLocation + (ForwardVector * 250.0f);
	
	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		InteractionHitResult,
		CurrentLocation,
		TargetLocation,
		ECC_INTERACTION,
		QueryParams
		);

#if WITH_EDITOR
	DrawDebugLine(GetWorld(), CurrentLocation, TargetLocation, FColor::Red, false, 3.0f, 0, 1.0f);
#endif
	
	// 상호작용 오브젝트를 캐스팅하여 Interact 함수를 호출합니다.
	if (bHit)
	{
		LOG(TEXT(" Interaction Hit : %s"), *InteractionHitResult.GetActor()->GetName());
		InteractableObject = Cast<IInteractable>(InteractionHitResult.GetActor());
		IInteractable::Execute_Interact(InteractableObject->_getUObject());
	}
}
