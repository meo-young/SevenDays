#pragma once

#include "CoreMinimal.h"
#include "LevelSequenceActor.h"
#include "Interface/Interactable.h"
#include "SDLevelSequenceActor.generated.h"

class UBoxComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSequenceStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSequenceEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);

UCLASS()
class SEVENDAYS_API ASDLevelSequenceActor : public ALevelSequenceActor, public IInteractable
{
	GENERATED_BODY()

public:
	ASDLevelSequenceActor(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
public:
	/** LevelSequence가 시작될 때 호출되는 델리게이트 */
	UPROPERTY(BlueprintAssignable)
	FOnSequenceStarted OnSequenceStartedDelegate;

	/** LevelSequence가 종료될 때 호출되는 델리게이트 */
	UPROPERTY(BlueprintAssignable)
	FOnSequenceEnded OnSequenceEndedDelegate;

	/** 상호작용이 발생할 때 호출되는 델리게이트 */
	UPROPERTY(BlueprintAssignable)
	FOnInteract OnInteractDelegate;
	
public:
	/** 등록된 LevelSequence를 재생하는 함수 */
	UFUNCTION(BlueprintCallable)
	void PlayLevelSequence();
	
	/** 등록된 LevelSequence를 루프로 재생하는 함수 */
	UFUNCTION(BlueprintCallable)
	void PlayLevelSequenceForLoop();

	/** 등록된 LevelSequence를 루프로 재생하는 함수 */
	UFUNCTION(BlueprintCallable)
	void StopLevelSequence();
	
	/** LevelSequence의 종료 델리게이트에 바인딩할 함수 */
	UFUNCTION(BlueprintCallable)
	void OnSequenceEnded();

	/** IInteractable 인터페이스를 구현한 함수 */
	virtual void Interact_Implementation();

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> LevelSequence = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UBoxComponent* InteractionBox = nullptr;
	
};
