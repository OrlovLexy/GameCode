#include "Medkit.h"
#include "Characters/GCBaseCharacter.h"

bool UMedkit::Consume(AGCBaseCharacter* ConsumeTarget)
{
	ConsumeTarget->AddHealth(Health);
	this->ConditionalBeginDestroy();
	return true;
}
