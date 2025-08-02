#include "UI/CrosshairWidget.h"

void UCrosshairWidget::ShowCrosshair()
{
	if (!IsInViewport())
	{
		AddToViewport();
	}
}
