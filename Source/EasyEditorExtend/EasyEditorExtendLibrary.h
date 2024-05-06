// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" 
#include "AssetRegistry/AssetRegistryModule.h"
#include "ClassesExtend/DynamicButtonEditorExtendExecuteCode.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EasyEditorExtendLibrary.generated.h"

/**
 * 
 */


class UEditorUtilityWidgetBlueprint;
class UEditorUtilityWidget;
UCLASS()
class EASYEDITOREXTEND_API UEasyEditorExtendLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static FToolMenuSection& GetToolbarMenuSection(const FName MenuPath, const FName SectionName)
	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu(MenuPath);
		return ToolbarMenu->FindOrAddSection(SectionName);
	}

	static FToolMenuSection& GetPluginToolsSection()
	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		return ToolbarMenu->FindOrAddSection("PluginTools");
	}

	static UToolMenu* GetExtendMenu(const FName MenuPath)
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu(MenuPath);
		if(Menu)
		{
			return Menu;
		}else
		{
			UE_LOG(LogTemp, Warning, TEXT("UEasyEditorExtendLibrary: Menu not found! ") );
			return nullptr;
		}
	}

	UFUNCTION(BlueprintCallable)
	static bool GetBlueprintsFromBaseClass(UClass* InBaseClass,
															 TArray<FAssetData>& FoundAssets)
	{
		if (!IsValid(InBaseClass))
		{
			return false;
		}
		
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(
			"AssetRegistry");

		TMultiMap<FName, FString> TagsValues;

		// All native CDOs should be in memory - we can look up native subclasses of InNativeClass
		for (FThreadSafeObjectIterator It(InBaseClass); It; ++It)
		{
			const UClass* Class = It->GetClass();
			if (Class->IsNative() && Class->ClassDefaultObject == *It)
			{
				TagsValues.AddUnique(FBlueprintTags::NativeParentClassPath,
									 FObjectPropertyBase::GetExportPath(Class));
			}
		}

		return AssetRegistryModule.Get().GetAssetsByTagValues(TagsValues, FoundAssets);
	}
	UFUNCTION(BlueprintCallable)
	static TArray<UClass*> GetClassFromAssetData(TArray<FAssetData>& InAssetData)
	{
		TArray<UClass*> ClassArrayOut;
		ClassArrayOut.Empty();
		// Iterate over retrieved blueprint assets
		for (FAssetData asset : InAssetData)
		{
			FString ObjPath = asset.ObjectPath.ToString().Append("_C");
			//UE_LOG(LogTemp, Warning, TEXT("Object path is : %s"), *ObjPath);
			//UE_LOG(LogTemp, Warning, TEXT("Trying to return BPClass!"));

			//Get UClass
			ClassArrayOut.Add(StaticLoadClass(UObject::StaticClass(), NULL, *ObjPath, NULL, LOAD_None, NULL));
		}

		return ClassArrayOut;

	}

	UFUNCTION(BlueprintCallable)
	void OpenAssetByAssetData(FAssetData AssetData);
	UFUNCTION(BlueprintCallable)
	static void RunEditorUtility(UEditorUtilityWidget* Widget);
	UFUNCTION(BlueprintCallable)
	static void RunEditorUtilityBlueprint(UEditorUtilityWidgetBlueprint* Widget);
	UFUNCTION(BlueprintCallable)
	static void OpenMapInEditor(TSoftObjectPtr<UWorld> Map);

};
