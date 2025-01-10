#include "RPG_DialogSystemEditor.h"
#include "AssetToolsModule.h"
#include "RPG_DialogSystemEditor/RPG_QuestAssetAction.h"

#define LOCTEXT_NAMESPACE "FRPG_DialogSystemEditorModule"

void FRPG_DialogSystemEditorModule::StartupModule()
{
	IAssetTools& AssetTools = IAssetTools::Get();
	EAssetTypeCategories::Type category = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("RPG_Systems")), LOCTEXT("RPG_Systems_AssetCategory", "RPG_Systems"));

	TSharedPtr<IAssetTypeActions> QuestAssetAction = MakeShared<RPG_QuestAssetAction>(category);
	AssetTools.RegisterAssetTypeActions(QuestAssetAction.ToSharedRef());
}

void FRPG_DialogSystemEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FRPG_DialogSystemEditorModule, RPG_DialogSystemEditor)