// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_QuestAssetAction.h"

#include "RPG_QuestEditorApp.h"
#include "RPG_DialogSystemRuntime/RPG_QuestAsset.h"

RPG_QuestAssetAction::RPG_QuestAssetAction(EAssetTypeCategories::Type category)
{
	_assetCategory = category;
}

FText RPG_QuestAssetAction::GetName() const
{
	return FText::FromString("RPG_QuestAsset");
}

UClass* RPG_QuestAssetAction::GetSupportedClass() const
{
	return URPG_QuestAsset::StaticClass();
}

FColor RPG_QuestAssetAction::GetTypeColor() const
{
	return  FColor::Red;
}

uint32 RPG_QuestAssetAction::GetCategories()
{
	return _assetCategory;
}

void RPG_QuestAssetAction::OpenAssetEditor(const TArray<UObject*>& InObjects,
	const EAssetTypeActivationOpenedMethod OpenedMethod, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* Object : InObjects)
	{
		URPG_QuestAsset* QuestAsset = Cast<URPG_QuestAsset>(Object);
		if (QuestAsset != nullptr)
		{
			TSharedRef<RPG_QuestEditorApp> editor(new RPG_QuestEditorApp());
			editor->InitEditor(Mode, EditWithinLevelEditor, QuestAsset);
		}
	}
}
