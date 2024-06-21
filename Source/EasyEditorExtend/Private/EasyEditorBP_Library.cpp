// Fill out your copyright notice in the Description page of Project Settings.


#include "EasyEditorExtend/Public/EasyEditorBP_Library.h"

UWorld* UEasyEditorBP_Library::GetCurrentEditorWorld()
{
	if (GEditor)
	{
		FWorldContext& EditorWorldContext = GEditor->GetEditorWorldContext();
		return EditorWorldContext.World();
	}
	return nullptr;
}
