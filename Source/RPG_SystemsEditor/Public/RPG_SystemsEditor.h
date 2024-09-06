#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "EasyEditorExtend/EasyEditorExtendLibrary.h"
#include "RPG_Systems/InventorySystem/RPG_ItemData.h"
#include "RPG_SystemsEditor/SRPG_SystemsEditorWindow.h"

class FRPG_SystemsEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
    static TSharedPtr<class SRPG_SystemsEditorWindow> RPG_SystemsEditorWindow;
    static TSharedPtr<class SWindow> MyWindow;

    void RegisterMenuExtensions()
    {
        return;
        // Use the current object as the owner of the menus
        // This allows us to remove all our custom menus when the 
        // module is unloaded (see ShutdownModule below)
        FToolMenuOwnerScoped OwnerScoped(this);
        
        FToolMenuSection& ToolbarSection = UEasyEditorExtendLibrary::GetPluginToolsSection();
 
        ToolbarSection.AddEntry(FToolMenuEntry::InitToolBarButton(
            TEXT("MyCustomButtonName"),
            FExecuteAction::CreateLambda([this]()
            {
                // Simply log for this example
                UE_LOG(LogTemp, Warning, TEXT("Tentando abrir janela do editor RPG !!"));
                CreateRPG_EditorWindow();
                
                
            }),
            INVTEXT("My custom button"),
            INVTEXT("Tooltip for my custom button")
        ));

        CreateSubMenu();
    }

    void CreateSubMenu()
    {
        
        return;
        // Use the current object as the owner of the menus
        // This allows us to remove all our custom menus when the 
        // module is unloaded (see ShutdownModule below)
        FToolMenuOwnerScoped OwnerScoped(this);
        
        // Função para pegar a seção do editor mais facilmente 
        FToolMenuSection& ToolbarSection = UEasyEditorExtendLibrary::GetPluginToolsSection();

        FUIAction MyButtonAction = FUIAction(
        FExecuteAction::CreateLambda([]() {
            // Ação a ser executada quando o botão for clicado
            UE_LOG(LogTemp, Log, TEXT("Botão suspenso clicado!"));
            })
        );

        FToolMenuEntry ComboButtonEntry = FToolMenuEntry::InitComboButton(
            TEXT("RPG Tools"),
            FUIAction(),
            FOnGetContent::CreateLambda([=]()
        {
            // Crie e retorne o conteúdo do submenu aqui
            FMenuBuilder MenuBuilder(true, nullptr);

            for (auto Element : URPG_ItemData::GetAllItems())
            {
                FString a = Element->GetName();
                MenuBuilder.AddMenuEntry(FText::FromString(a), FText(), FSlateIcon(), MyButtonAction);
                
            }
                //MenuBuilder.AddMenuEntry(FText::FromString(TEXT("Show Log")), FText(), FSlateIcon(), MyButtonAction);
            //MenuBuilder.AddMenuEntry(FText::FromString(TEXT("Option 2")), FText(), FSlateIcon(), FUIAction());
            // Adicione mais opções conforme necessário
            return MenuBuilder.MakeWidget();
        }),
            FText::FromString(TEXT("RPG Systems Tools")), // Texto do botão
            FText::FromString(TEXT("RPG Systems Tools")) ,// Tooltip do botão
            FSlateIcon(),
            true //bInSimpleComboBox
        );
        ToolbarSection.AddEntry(ComboButtonEntry);
    }                          

    
    void CreateRPG_EditorWindow();
    void OnRPG_EditorWindowClosed(const TSharedRef<SWindow>& Window);
    TSharedRef<SDockTab> OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs);
};
