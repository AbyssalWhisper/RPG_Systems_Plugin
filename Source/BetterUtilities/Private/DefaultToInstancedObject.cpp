// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultToInstancedObject.h"

UWorld* UDefaultToInstancedObject::GetWorld() const
{
	if (GetOuter() == nullptr)	return nullptr;

	if (Cast<UGameInstance>(GetOuter())) return Cast<UGameInstance>(GetOuter())->GetWorld();
	
	
	if (Cast<UPackage>(GetOuter()) != nullptr)
	{
		return Cast<UWorld>(GetOuter()->GetOuter());
	}
	if (GetOwningActor()) return GetOwningActor()->GetWorld();
	return nullptr;
}

AActor* UDefaultToInstancedObject::GetOwningActor() const
{
	return GetTypedOuter<AActor>();
}



void UDefaultToInstancedObject::DestroyObject()
{
	if (IsValid(this))
	{
#if ENGINE_MAJOR_VERSION == 5
		MarkAsGarbage();
#endif

#if ENGINE_MAJOR_VERSION == 4
		MarkPendingKill();
#endif

		OnDestroyed();
	}
}