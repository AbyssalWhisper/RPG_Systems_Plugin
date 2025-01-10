// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_QuestAssetAppMode.h"
#include "RPG_QuestAssetEditorPrimaryTabFactory.h"
#include "RPG_QuestEditorApp.h"

RPG_QuestAssetAppMode::RPG_QuestAssetAppMode(
TSharedPtr<class RPG_QuestEditorApp> app)  :
/* Identificador único para esse modo */
FApplicationMode("RPG_QuestAssetAppMode")
{
	_app = app;
	_tabs.RegisterFactory(MakeShareable(new RPG_QuestAssetEditorPrimaryTabFactory(app)));

	TabLayout = FTabManager::NewLayout("RPG_QuestAssetAppMode_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->AddTab("RPG_QuestAssetPrimaryTab", ETabState::OpenedTab)
			)
		);
}

void RPG_QuestAssetAppMode::RegisterTabFactories(
	TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<RPG_QuestEditorApp> app = _app.Pin();
	app->PushTabFactories(_tabs);
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void RPG_QuestAssetAppMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void RPG_QuestAssetAppMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
