#include "AnimNotify/AnimNotify_FootStep.h"
#include "Define/DefineClass.h"
#include "Subsystem/SoundSubsystem.h"

void UAnimNotify_FootStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	// Notify를 호출한 메시의 소유자를 가져온다.
	const AActor* Owner = MeshComp->GetOwner();
	if (!IsValid(Owner)) return;

	const UWorld* World = Owner->GetWorld();
	if (!IsValid(World)) return;

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!IsValid(GameInstance)) return;
	
	USoundSubsystem* SoundSubSystem = GameInstance->GetSubsystem<USoundSubsystem>();
	if (!IsValid(SoundSubSystem)) return;

	// PhysicalMaterial을 가져오기 위한 쿼리 파라미터를 설정한다.
	QueryParams.bReturnPhysicalMaterial = true;
	
	// 액터의 -150.0f 아래 위치를 계산한다.
	const FVector CurrentLocation = Owner->GetActorLocation();
	const FVector DownVector = Owner->GetActorUpVector() * -200.0f;
	const FVector TargetLocation = CurrentLocation + DownVector;
	const FRotator Rotator = Owner->GetActorRotation();

	// LineTrace 실시한다.
	const bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		CurrentLocation,
		TargetLocation,
		ECC_Visibility,
		QueryParams
		);
	
	// Surface Type에 따른 발걸음 소리를 재생한다.
	if (bHit)
	{
		SoundSubSystem->PlaySFX(ESFX::FootStep, HitResult.ImpactPoint);
	}
}
