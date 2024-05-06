// Fill out your copyright notice in the Description page of Project Settings.


#include "SRPG_SystemsEditorWindow.h"
#include "RPG_SystemsEditor.h"
#include "RPG_Systems/InventorySystem/RPG_ItemData.h"
#include "Blueprint/UserWidget.h"
#include "Editor/EditorEngine.h"
#include "Tests/AutomationCommon.h"

void SRPG_SystemsEditorWindow::Construct(const FArguments& InArgs)
{
	/*
	 Horizontal = SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SButton)
				.OnClicked(this, &SRPG_SystemsEditorWindow::OnButtonClick)
				.Text(FText::FromString(TEXT("Clique Aqui!")))
			];
		this->SetContent(Horizontal.ToSharedRef());
*/
	
	// Registrar um delegate para ser notificado quando um mundo for inicializado
	if(GEditor&&GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport))
	{
		FWorldContext* world = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		
		InitWidget(world->World());
		
	}
	else
	{
		FWorldDelegates::OnPostWorldInitialization.AddRaw(this, &SRPG_SystemsEditorWindow::OnWorldInitialized);
	}
	

}

FReply SRPG_SystemsEditorWindow::OnButtonClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Botão Clicado!"));
	CreateItemsSlot();
	return FReply::Handled();
}

FReply SRPG_SystemsEditorWindow::OnAssetSelected()
{
	return FReply::Handled();
}

void SRPG_SystemsEditorWindow::CreateItemsSlot()
{
	if (!Horizontal)return;
	
	Horizontal->ClearChildren();
	auto items = URPG_ItemData::GetAllItems();
	for (auto* Item : items)
	{
		FString ItemName = Item->GetName();
		Horizontal->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SButton)
			.OnClicked(this, &SRPG_SystemsEditorWindow::OnAssetSelected)
			.Text(FText::FromString(ItemName))
		];
	}
}

void SRPG_SystemsEditorWindow::OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS)
{
	
	InitWidget(World);
}

void SRPG_SystemsEditorWindow::InitWidget(UWorld* World)
{
	if (World)
	{
		// Carregar o widget Blueprint
		UClass* WidgetBPClass = LoadClass<UUserWidget>(nullptr, TEXT("/RPG_Systems/testwidget.testwidget_C"));
		if (WidgetBPClass)
		{
			// Criar uma instância do widget Blueprint
			UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(World, WidgetBPClass);

			if (WidgetInstance)
			{
				// Definir o conteúdo da janela como o widget
				SetContent(WidgetInstance->TakeWidget());

			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to create instance of Widget Blueprint"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load Widget Blueprint class"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get reference to the world"));
	}
}

