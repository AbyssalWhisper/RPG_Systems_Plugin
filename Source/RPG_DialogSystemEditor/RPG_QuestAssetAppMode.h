// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"
/**
 * 
 */
class RPG_QuestAssetAppMode : public FApplicationMode
{
public:
	RPG_QuestAssetAppMode(TSharedPtr<class RPG_QuestEditorApp> app);
	
	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

private:
	TWeakPtr<class RPG_QuestEditorApp> _app;
	FWorkflowAllowedTabSet _tabs;
	
};
