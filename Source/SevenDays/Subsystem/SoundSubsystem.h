#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundSubsystem.generated.h"

struct FBGMDataTableRow;
struct FSFXDataTableRow;
enum class EBGM : uint8;
enum class ESFX : uint8;

UCLASS()
class SEVENDAYS_API USoundSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USoundSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	/** PlayAtLocation으로 사운드를 재생한다. */
	void PlaySFX(ESFX SFX, const FVector& InLocation);

	/** Play2D로 사운드를 재생한다. */
	void PlaySFX2D(ESFX SFX);

	/** Loop로 BGM을 재생한다. */
	UFUNCTION(BlueprintCallable)
	void PlayBGM(EBGM BGM);

	/** 현재 재생중인 BGM을 중단한다. */
	UFUNCTION(BlueprintCallable)
	void StopBGM();

private:
	/** SFX 데이터 테이블 */
	UPROPERTY()
	TObjectPtr<UDataTable> SFXDataTable;

	/** SFX 데이터 테이블에서 추출한 모든 행 */
	TArray<FSFXDataTableRow*> SFXDataRows;

	/** BGM 데이터 테이블 */
	UPROPERTY()
	TObjectPtr<UDataTable> BGMDataTable;

	/** BGM 데이터 테이블에서 추출한 모든 행 */
	TArray<FBGMDataTableRow*> BGMDataRows;
	
	UPROPERTY()
	UAudioComponent* CurrentBGMAudioComponent = nullptr;
};
