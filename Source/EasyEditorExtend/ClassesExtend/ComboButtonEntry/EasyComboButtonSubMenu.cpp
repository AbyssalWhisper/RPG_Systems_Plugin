// Fill out your copyright notice in the Description page of Project Settings.


#include "EasyComboButtonSubMenu.h"

void UEasyComboButtonSubMenu::CreateSubMenu(FMenuBuilder& MenuBuilder)
{
	for (auto Element : SubMenuList)
	{
		if (Element)
		{
			Element->Execute(MenuBuilder);
		}
	}
}

void UEasyComboButtonSubMenu::Execute(FMenuBuilder& MenuBuilder)
{
	Super::Execute(MenuBuilder);
	MenuBuilder.AddSubMenu(
		ButtonLabel,
		ButtonTooltip,
		FNewMenuDelegate::CreateUObject(this,&UEasyComboButtonSubMenu::CreateSubMenu) 
	);
	
}
