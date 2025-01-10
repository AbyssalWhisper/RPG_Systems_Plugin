// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_QuestAssetFactory.h"

#include "RPG_DialogSystemRuntime/RPG_QuestAsset.h"

URPG_QuestAssetFactory::URPG_QuestAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = URPG_QuestAsset::StaticClass();
}
UObject* URPG_QuestAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn)
{
	URPG_QuestAsset* NewQuestAsset = NewObject<URPG_QuestAsset>(InParent, InClass, InName, Flags);
	return NewQuestAsset;
}

bool URPG_QuestAssetFactory::CanCreateNew() const
{
	return true;
}
