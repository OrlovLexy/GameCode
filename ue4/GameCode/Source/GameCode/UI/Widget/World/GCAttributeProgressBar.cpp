#include "GCAttributeProgressBar.h"
#include "Components/ProgressBar.h"

void UGCAttributeProgressBar::SetProgressPercentage(float Percentage)
{
	HealthProgressBar->SetPercent(Percentage);
}
