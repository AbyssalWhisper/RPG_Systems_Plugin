#include "EasyEditorExtend.h"

#include "EasyEditorExtend/EasyEditorExtendLibrary.h"
#include "EasyEditorExtend/ClassesExtend/EasyEditorObjectExecuteCode.h"
#include "EasyEditorExtend/ClassesExtend/ComboButtonEntry/EasyComboButtonComponent.h"

#include "EasyEditorExtend/ClassesExtend/EasyEditorObjectAutoExecuteCode.h"

#define LOCTEXT_NAMESPACE "FEasyEditorExtendModule"

void FEasyEditorExtendModule::RegisterMenuExtensions()
{
	
}

void FEasyEditorExtendModule::OnWorldInitialized(UWorld* World, FWorldInitializationValues WorldInitializationValues)
{
	



	for (auto Element : GetAllEasyEditorAutoExecuteCode())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %s "), *Element->GetName());
		if (Element && GEditor)
		{
			EasyEditorAutoExecuteCode.Add(Element);
			UE_LOG(LogTemp, Warning, TEXT("Try Execute Code %s"), *Element->GetName());
			if (Element->CustomExecuteCode)
			{
				Element->CustomExecuteCode->Execute();
			}
		}
	}
}

void FEasyEditorExtendModule::StartupModule()
{
	// Register a function to be called when menu system is initialized
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(
		this, &FEasyEditorExtendModule::RegisterMenuExtensions));
	// Registrar o evento de inicialização do mundo
	UE_LOG(LogTemp, Warning, TEXT("EasyEditorExtendModule Initialized"));
	//FWorldDelegates::
}

void FEasyEditorExtendModule::ShutdownModule()
{
		
	// Unregister the startup function
	UToolMenus::UnRegisterStartupCallback(this);
 
	// Unregister all our menu extensions
	UToolMenus::UnregisterOwner(this);
	UE_LOG(LogTemp, Warning, TEXT("EasyEditorExtendModule Shutdown"));
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FEasyEditorExtendModule, EasyEditorExtend)