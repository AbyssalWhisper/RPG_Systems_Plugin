// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
/**
 * 
 */
class RPG_SYSTEMSEDITOR_API SRPG_SystemsEditorWindow: public SWindow
{
public:
	SLATE_BEGIN_ARGS(SRPG_SystemsEditorWindow ) 
	{}
		
	SLATE_END_ARGS()

	TSharedPtr<SVerticalBox> Horizontal;

	void Construct(const FArguments& InArgs);
	FReply  OnButtonClick();
	FReply  OnAssetSelected();
	
	void CreateItemsSlot();
	void OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS);

	void InitWidget(UWorld* World);
};

