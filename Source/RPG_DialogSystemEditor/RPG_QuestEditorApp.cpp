// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_QuestEditorApp.h"

#include "RPG_QuestAssetAppMode.h"

void RPG_QuestEditorApp::RegisterTabSpawners(const TSharedRef<class FTabManager>& inTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(inTabManager);
}

void RPG_QuestEditorApp::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UObject* InObject)
{
	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(InObject);

	InitAssetEditor(Mode, InitToolkitHost,
		TEXT("QuestAssetEditor"),
		FTabManager::FLayout::NullLayout,
		true,
		true,
		ObjectsToEdit);

	AddApplicationMode("RPG_QuestAssetAppMode",
		MakeShareable(new RPG_QuestAssetAppMode(SharedThis(this))));
	SetCurrentMode("RPG_QuestAssetAppMode");
}

void RPG_QuestEditorApp::OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit)
{
	FWorkflowCentricApplication::OnToolkitHostingFinished(Toolkit);
}

void RPG_QuestEditorApp::OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit)
{
	FWorkflowCentricApplication::OnToolkitHostingStarted(Toolkit);
}
