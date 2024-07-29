// Copyright Zurichi


#include "BetterUtilities/Objects/ConditionObjects/AndConditionObject.h"

bool UAndConditionObject::CheckCondition_Implementation(AActor* Actor)
{
	return IsAllTrueCondition(ConditionObjects,Actor);
}
