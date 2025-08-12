// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicButtonEditorExtend.h"

#include "EasyEditorObjectExecuteCode.h"
#include "EasyEditorExtend/EasyEditorExtendLibrary.h"

void UDynamicButtonEditorExtend::Execute()
{
	if (CustomExecuteCode)
	{
		CustomExecuteCode->Run();
	}
}

void UDynamicButtonEditorExtend::CreateDymamicButtons()
{
	for (auto Element : GetAllGetAllDynamicButtonExtend())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %s "), *Element->GetName());
		if (Element && GEditor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Try Create %s Button"), *Element->GetName());
			CreateButton(Element);
		}
	}
}

void UDynamicButtonEditorExtend::CreateButton(UDynamicButtonEditorExtend* ButtonObject)
{
	  
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