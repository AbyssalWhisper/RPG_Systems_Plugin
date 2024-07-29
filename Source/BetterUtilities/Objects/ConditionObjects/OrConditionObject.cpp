// Copyright Zurichi


#include "BetterUtilities/Objects/ConditionObjects/OrConditionObject.h"

bool UOrConditionObject::CheckCondition_Implementation(AActor* Actor)
{
	return HasAnyTrueCondition(ConditionObjects,Actor);
}
