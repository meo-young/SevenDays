#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

class UTexture2D;

UCLASS()
class SEVENDAYS_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Crosshair를 화면에 표시하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void ShowCrosshair();

	/** Crosshair가 정상 상태임을 표시하는 함수입니다. */
	UFUNCTION(BlueprintImplementableEvent)
	void NormalCrosshair();

	/** Crosshair가 로딩 중임을 표시하는 함수입니다. */
	UFUNCTION(BlueprintImplementableEvent)
	void LoadingCrosshair();
	
};
