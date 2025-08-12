// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicComboButtonExtend.h"

#include "ComboButtonEntry/EasyComboButtonComponent.h"
#include "EasyEditorExtend/EasyEditorExtendLibrary.h"

void UDynamicComboButtonExtend::CreateDymamicButtons()
{
	for (auto Element : GetAllDynamicComboButtonExtend())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %s "), *Element->GetName());
		if (Element && GEditor)
		{
			//DynamicComboButtonExtend.Add(Element);
			UE_LOG(LogTemp, Warning, TEXT("Try Create %s Combo Button"), *Element->GetName());
			CreateComboButton(Element);
		}
	}
}

void UDynamicComboButtonExtend::CreateComboButton(UDynamicComboButtonExtend* ButtonObject)
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
