#pragma once

#include <string>

#include "CoreMinimal.h"
#include "Algo/ForEach.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "EasyEditorExtend/EasyEditorExtendLibrary.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "EasyEditorExtend/ClassesExtend/DynamicButtonEditorExtend.h"
#include "EasyEditorExtend/ClassesExtend/DynamicComboButtonExtend.h"
#include "Modules/ModuleManager.h"

class FEasyEditorExtendModule : public IModuleInterface
{
public:
    void RegisterMenuExtensions();
    void OnWorldInitialized(UWorld* World, FWorldInitializationValues WorldInitializationValues);
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
	bool Initialized = false;
    TArray<UDynamicButtonEditorExtend*> DynamicButtonEditor;
	TArray<UDynamicComboButtonExtend*> DynamicComboButtonExtend;
    
    TArray<UClass*> GetAllClasses()
    {
        TArray<FAssetData> AssetDataList;
        FARFilter SearchFilter;
        //SearchFilter.ClassNames.Add(UDynamicButtonEditorExtend::StaticClass()->GetFName());
        //SearchFilter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
        //SearchFilter.ClassNames.Add(UBlueprintGeneratedClass::StaticClass()->GetFName());
        
       //FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
        //AssetRegistryModule.Get().GetAssets(SearchFilter, AssetDataList, true);
    	UEasyEditorExtendLibrary::GetBlueprintsFromBaseClass(UDynamicButtonEditorExtend::StaticClass(),AssetDataList);
        
        return UEasyEditorExtendLibrary::GetClassFromAssetData(AssetDataList);
    }

	
	static TArray<UDynamicButtonEditorExtend*> GetAllGetAllDynamicButtonExtend()
    {
    	TArray<UDynamicButtonEditorExtend*> Items;
    	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    	TArray<FAssetData> AssetData;
    	FTopLevelAssetPath ClassPathName = FTopLevelAssetPath("/Script/EasyEditorExtend","DynamicButtonEditorExtend");
		
    	AssetRegistryModule.Get().GetAssetsByClass(ClassPathName, AssetData,true);
    	for (int i = 0; i < AssetData.Num(); i++) {
    		UDynamicButtonEditorExtend* FoundAsset = Cast<UDynamicButtonEditorExtend>(AssetData[i].GetAsset());
			
    		if (FoundAsset != NULL) {
    			Items.Add(FoundAsset);
    		}
    	}
    	/*
		for (TObjectIterator<URPG_ItemData> It; It; ++It)
		{
			Items.Add(*It);
		}*/
    	return Items;
    }

	static TArray<UDynamicComboButtonExtend*> GetAllDynamicComboButtonExtend()
    {
    	TArray<UDynamicComboButtonExtend*> Items;
    	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    	TArray<FAssetData> AssetData;
    	FTopLevelAssetPath ClassPathName = FTopLevelAssetPath("/Script/EasyEditorExtend","DynamicComboButtonExtend");
		
    	AssetRegistryModule.Get().GetAssetsByClass(ClassPathName, AssetData,true);
    	for (int i = 0; i < AssetData.Num(); i++) {
    		UDynamicComboButtonExtend* FoundAsset = Cast<UDynamicComboButtonExtend>(AssetData[i].GetAsset());
			
    		if (FoundAsset != NULL) {
    			Items.Add(FoundAsset);
    		}
    	}
    	/*
		for (TObjectIterator<URPG_ItemData> It; It; ++It)
		{
			Items.Add(*It);
		}*/
    	return Items;
    }
    void CreateButton(UDynamicButtonEditorExtend* ButtonObject);
	void CreateComboButton(UDynamicComboButtonExtend* ButtonObject);


};
