#include "LevelSequence/SDLevelSequenceActor.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "SevenDays.h"
#include "Components/BoxComponent.h"
#include "Define/DefineClass.h"

ASDLevelSequenceActor::ASDLevelSequenceActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetupAttachment(RootComponent);
	InteractionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionBox->SetCollisionResponseToChannel(ECC_INTERACTION, ECR_Block);
}

void ASDLevelSequenceActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 등록된 레벨 시퀀스를 추출한다.
	LevelSequence = GetSequence();
	if (!IsValid(LevelSequence))
	{
		LOG(TEXT("LevelSequence Is Not Valid !"));
		return;
	}

	if (LevelSequencePlayer) return;

	// 재생 전에 레벨 시퀀스 플레이어를 변수화 해야 종료 델리게이트에서 활용할 수 있다.
	ALevelSequenceActor* OutActor;
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, FMovieSceneSequencePlaybackSettings(), OutActor);
}

void ASDLevelSequenceActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(LevelSequencePlayer))
	{
		// 종료 델리게이트에 함수를 바인딩한다.
		LevelSequencePlayer->OnFinished.AddDynamic(this, &ThisClass::OnSequenceEnded);	
	}
}

void ASDLevelSequenceActor::PlayLevelSequence()
{
	LOG(TEXT("%s PlayLevelSequence"), *GetName());
	if (OnSequenceStartedDelegate.IsBound())
	{
		OnSequenceStartedDelegate.Broadcast();
	}

	LevelSequencePlayer->Play();
}

void ASDLevelSequenceActor::PlayLevelSequenceForLoop()
{
	LOG(TEXT("%s PlayLoopLevelSequence"), *GetName());
	if (OnSequenceStartedDelegate.IsBound())
	{
		OnSequenceStartedDelegate.Broadcast();
	}

	InteractionBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);

	LevelSequencePlayer->PlayLooping();
}

void ASDLevelSequenceActor::StopLevelSequence()
{
	InteractionBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	LevelSequencePlayer->Stop();
}

void ASDLevelSequenceActor::OnSequenceEnded()
{
	if (OnSequenceEndedDelegate.IsBound())
	{
		OnSequenceEndedDelegate.Broadcast();
	}
}

void ASDLevelSequenceActor::Interact_Implementation()
{
	if (OnInteractDelegate.IsBound())
	{
		OnInteractDelegate.Broadcast();
	}

	StopLevelSequence();

	OnInteractDelegate.Clear();
}
