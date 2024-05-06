// Fill out your copyright notice in the Description page of Project Settings.


#include "EasyEditorExtendLibrary.h"

#include "EditorAssetLibrary.h"
#include "EditorUtilityLibrary.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "FileHelpers.h"

void UEasyEditorExtendLibrary::OpenAssetByAssetData(FAssetData AssetData)
{

}

void UEasyEditorExtendLibrary::RunEditorUtility(UEditorUtilityWidget* Widget)
{
	if (Widget)
	{
	Widget->Run();
	}
}

void UEasyEditorExtendLibrary::RunEditorUtilityBlueprint(UEditorUtilityWidgetBlueprint* Widget)
{
	if (Widget)
	{
		UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
		if (!EditorUtilitySubsystem)return;
		EditorUtilitySubsystem->SpawnAndRegisterTab(Widget);
	}
}

void UEasyEditorExtendLibrary::OpenMapInEditor(TSoftObjectPtr<UWorld> Map)
{
	const FString LevelName = FPackageName::ObjectPathToPackageName(Map.ToString());
	FEditorFileUtils::LoadMap(LevelName);
	if (Map)
	{
		UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
		if (AssetEditorSubsystem)
		{
			
			
			UE_LOG(LogTemp, Log, TEXT("MapName: %s") , *Map->GetPathName());
			AssetEditorSubsystem->OpenEditorForAsset(Map->GetPathName());
		} 
	}
}
