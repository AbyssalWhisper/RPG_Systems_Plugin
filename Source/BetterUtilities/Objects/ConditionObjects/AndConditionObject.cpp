// Fill out your copyright notice in the Description page of Project Settings.


#include "BetterUtilities/Objects/ConditionObjects/AndConditionObject.h"

bool UAndConditionObject::CheckCondition_Implementation(AActor* Actor)
{
	return IsAllTrueCondition(ConditionObjects,Actor);
}
