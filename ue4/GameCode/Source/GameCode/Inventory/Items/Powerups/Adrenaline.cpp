#include "Adrenaline.h"
#include "Characters/GCBaseCharacter.h"

bool UAdrenaline::Consume(AGCBaseCharacter* ConsumeTarget)
{
	ConsumeTarget->RestoreFullStamina();
	this->ConditionalBeginDestroy();
	return true;
}
