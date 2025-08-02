#include "Subsystem/SoundSubsystem.h"

#include "SevenDays.h"
#include "Components/AudioComponent.h"
#include "DataTableRow/BGMDataTableRow.h"
#include "DataTableRow/SFXDataTableRow.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

USoundSubsystem::USoundSubsystem()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SFX(TEXT("/Game/_SevenDays/DataTable/DT_SFX"));
	if (DT_SFX.Succeeded())
	{
		SFXDataTable = DT_SFX.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_BGM(TEXT("/Game/_SevenDays/DataTable/DT_BGM"));
	if (DT_BGM.Succeeded())
	{
		BGMDataTable = DT_BGM.Object;
	}
}

void USoundSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// SFX와 BGM 데이터 테이블에서 모든 행을 가져옵니다.
	SFXDataTable->GetAllRows<FSFXDataTableRow>(TEXT(""), SFXDataRows);
	BGMDataTable->GetAllRows<FBGMDataTableRow>(TEXT(""), BGMDataRows);
}

void USoundSubsystem::PlaySFX(ESFX SFX, const FVector& InLocation)
{
	// SFXDataRows가 유효한지 확인한다.
	if (SFXDataRows.Num() <= 0)
	{
		LOG(TEXT("SFX Data Table이 유효하지 않습니다."));
		return;
	}

	// SFX Enum에 해당하는 사운드를 가져온다.
	USoundCue* SoundCue = SFXDataRows[static_cast<int32>(SFX)]->SoundCue;
	if (!IsValid(SoundCue))
	{
		LOG(TEXT("SFX Data Table에 정의된 SoundCue가 유효하지 않습니다."));
		return;
	}

	// 0.8f에서 1.2f 사이의 랜덤 볼륨을 지정한다.
	const float VolumeMultiplier = FMath::RandRange(0.8f, 1.2f);

	// 공간 음향을 재생한다.
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundCue, InLocation, FRotator::ZeroRotator, VolumeMultiplier, 1.0f, 0.0f);
}

void USoundSubsystem::PlaySFX2D(ESFX SFX)
{
	// SFXDataRows가 유효한지 확인한다.
	if (SFXDataRows.Num() <= 0)
	{
		LOG(TEXT("SFX Data Table이 유효하지 않습니다."));
		return;
	}

	// SFX Enum에 해당하는 사운드를 가져온다.
	USoundCue* SoundCue = SFXDataRows[static_cast<int32>(SFX)]->SoundCue;
	if (!IsValid(SoundCue))
	{
		LOG(TEXT("SFX Data Table에 정의된 SoundCue가 유효하지 않습니다."));
		return;
	}
	
	// UGameplayStatics를 사용하여 2D 사운드 재생
	UGameplayStatics::PlaySound2D(GetWorld(), SoundCue);
}

void USoundSubsystem::PlayBGM(EBGM BGM)
{
	if (BGMDataRows.Num() <= 0)
	{
		LOG(TEXT("BGM Data Table이 유효하지 않습니다."));
		return;
	}

	// SFX Enum에 해당하는 사운드를 가져온다.
	USoundCue* SoundCue = BGMDataRows[static_cast<int32>(BGM)]->SoundCue;
	if (!IsValid(SoundCue))
	{
		LOG(TEXT("BGM Data Table에 정의된 SoundCue가 유효하지 않습니다."));
		return;
	}
	
	// 기존 BGM 정지
	if (CurrentBGMAudioComponent && CurrentBGMAudioComponent->IsPlaying())
	{
		CurrentBGMAudioComponent->Stop();
	}

	// UGameplayStatics를 사용하여 BGM 재생
	CurrentBGMAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), SoundCue);
}

void USoundSubsystem::StopBGM()
{
	// 기존 BGM 정지
	if (CurrentBGMAudioComponent && CurrentBGMAudioComponent->IsPlaying())
	{
		CurrentBGMAudioComponent->Stop();
	}
}
