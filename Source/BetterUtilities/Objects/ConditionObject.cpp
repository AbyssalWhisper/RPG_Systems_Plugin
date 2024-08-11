// Fill out your copyright notice in the Description page of Project Settings.


#include "BetterUtilities/Objects/ConditionObject.h"

bool UConditionObject::IsAllTrueCondition(TArray<UConditionObject*> Objects, AActor* Instigator)
{
	for(auto Item : Objects)
	{
		if (Item) {
			if (Item->CheckCondition(Instigator) == false)return false;
		}
	}
	return true;
}

bool UConditionObject::HasAnyTrueCondition(TArray<UConditionObject*> Objects, AActor* Instigator)
{
	for (auto Item : Objects)
	{
		if (Item) {
			if (Item->CheckCondition(Instigator) == true)return true;
		}
	}
	return false;
}

bool UConditionObject::CheckCondition_Implementation(AActor* Actor)
{
	return false;
}
