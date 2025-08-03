#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "Door.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorInteract);

class ULevelSequencePlayer;
class UBoxComponent;
class ULevelSequence;

UCLASS()
class SEVENDAYS_API ADoor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ADoor();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	/** 상호작용 시 호출되는 델리게이트 */
	UPROPERTY(BlueprintAssignable)
	FOnDoorInteract OnInteractDelegate;

public:
	/** IInteractable 인터페이스를 구현한 함수 */
	virtual void Interact_Implementation();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LevelSequence")
	ULevelSequence* DoorOpenSequence = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LevelSequence")
	ULevelSequence* DoorLockedSequence = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	UBoxComponent* InteractionBox = nullptr;
	
private:
	/** 문을 열 수 있는 지 상태를 나타내는 변수입니다. */
	uint8 bIsCanOpen : 1 = false;

	/** LevelSequencePlayer 인스턴스입니다. */
	UPROPERTY()
	ULevelSequencePlayer* DoorOpenLevelSequencePlayer = nullptr;

	UPROPERTY()
	ULevelSequencePlayer* DoorLockedLevelSequencePlayer = nullptr;

};
