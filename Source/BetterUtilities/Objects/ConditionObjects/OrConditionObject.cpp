// Fill out your copyright notice in the Description page of Project Settings.


#include "BetterUtilities/Objects/ConditionObjects/OrConditionObject.h"

bool UOrConditionObject::CheckCondition_Implementation(AActor* Actor)
{
	return HasAnyTrueCondition(ConditionObjects,Actor);
}
