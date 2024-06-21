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
	if (MainWorld)
	{
		UE_LOG(LogTemp, Warning, TEXT("World Name %s"), *GetCurrentEditorWorld()->GetName());
	}
	else
	{
		MainWorld = World;
		UE_LOG(LogTemp, Warning, TEXT("World Name %s"), *GetCurrentEditorWorld()->GetName());

	}
	

	if (Initialized)return;
	Initialized = true;

	for (auto Element : GetAllGetAllDynamicButtonExtend())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %s "), *Element->GetName());
		if (Element && GEditor)
		{
				DynamicButtonEditor.Add(Element);
				UE_LOG(LogTemp, Warning, TEXT("Try Create %s Button"), *Element->GetName());
				CreateButton(Element);
		}
	}
	for (auto Element : GetAllDynamicComboButtonExtend())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %s "), *Element->GetName());
		if (Element && GEditor)
		{
			DynamicComboButtonExtend.Add(Element);
			UE_LOG(LogTemp, Warning, TEXT("Try Create %s Combo Button"), *Element->GetName());
			CreateComboButton(Element);
		}
	}

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
	FWorldDelegates::OnPostWorldInitialization.AddRaw(this, &FEasyEditorExtendModule::OnWorldInitialized);
	UE_LOG(LogTemp, Warning, TEXT("EasyEditorExtendModule Initialized"));
	//FWorldDelegates::
}

void FEasyEditorExtendModule::ShutdownModule()
{
		
	// Unregister the startup function
	UToolMenus::UnRegisterStartupCallback(this);
 
	// Unregister all our menu extensions
	UToolMenus::UnregisterOwner(this);
	DynamicButtonEditor.Empty();
	DynamicComboButtonExtend.Empty();
	// Remover o registro do evento ao desligar o módulo
	FWorldDelegates::OnPostWorldInitialization.RemoveAll(this);
	UE_LOG(LogTemp, Warning, TEXT("EasyEditorExtendModule Shutdown"));
}

void FEasyEditorExtendModule::CreateButton(UDynamicButtonEditorExtend* ButtonObject)
{
	FToolMenuOwnerScoped OwnerScoped(this);
        
	FToolMenuSection& ToolbarSection = UEasyEditorExtendLibrary::GetPluginToolsSection();

	FUIAction MyButtonAction = FUIAction(
		FExecuteAction::CreateLambda([ButtonObject]() {
			ButtonObject->Execute();
			})
		);
	
	ToolbarSection.AddEntry(FToolMenuEntry::InitToolBarButton(
		ButtonObject->ButtonName,MyButtonAction,
		//FExecuteAction::CreateUObject(ButtonObject->CustomExecuteCode,&UDynamicButtonEditorExtendExecuteCode::Execute),
		ButtonObject->ButtonLabel,
		ButtonObject->ButtonTooltip
	));

	
}

void FEasyEditorExtendModule::CreateComboButton(UDynamicComboButtonExtend* ButtonObject)
{
	if (!ButtonObject)return;
	FToolMenuSection& ToolbarSection = UEasyEditorExtendLibrary::GetPluginToolsSection();

	FToolMenuEntry ComboButtonEntry = FToolMenuEntry::InitComboButton(
			ButtonObject->ButtonName,
			FUIAction(),
			FOnGetContent::CreateLambda([=]()
		{
			FMenuBuilder MenuBuilder(true, nullptr);

			
			for (auto& Element : ButtonObject->ButtonsList)
			{
				if (Element)
				{
					Element->Execute(MenuBuilder);
				}
			}
			
			return MenuBuilder.MakeWidget();
		}),
			ButtonObject->ButtonLabel, // Button Label
			ButtonObject->ButtonTooltip ,// Button Tooltip
			FSlateIcon(FAppStyle::GetAppStyleSetName(),ButtonObject->IconID),
			ButtonObject->bInSimpleComboBox //bInSimpleComboBox
		);
	ToolbarSection.AddEntry(ComboButtonEntry);
}

UWorld* FEasyEditorExtendModule::GetCurrentEditorWorld()
{
	if (GEditor)
	{
		// Obtém o contexto do mundo do editor
		FWorldContext& EditorWorldContext = GEditor->GetEditorWorldContext();
		return EditorWorldContext.World();
	}
	return nullptr;
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FEasyEditorExtendModule, EasyEditorExtend)