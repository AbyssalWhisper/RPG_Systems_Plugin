// Fill out your copyright notice in the Description page of Project Settings.


#include "EasyComboButtonEntry.h"

#include "EasyEditorExtend/ClassesExtend/EasyEditorObjectExecuteCode.h"

void UEasyComboButtonEntry::Execute(FMenuBuilder& MenuBuilder)
{
	Super::Execute(MenuBuilder);
	if (CustomExecuteCode)
	{
		FUIAction MyButtonAction = FUIAction(
						FExecuteAction::CreateLambda([this]() {
						CustomExecuteCode->Run();
						})
					);
		MenuBuilder.AddMenuEntry(ButtonLabel, ButtonTooltip, FSlateIcon(), MyButtonAction);
	}
	
}
