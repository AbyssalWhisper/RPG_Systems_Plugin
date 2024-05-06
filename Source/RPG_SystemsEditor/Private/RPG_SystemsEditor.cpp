#include "RPG_SystemsEditor.h"
#include "CoreMinimal.h"



#define LOCTEXT_NAMESPACE "FRPG_SystemsEditorModule"

// Inicialização dos membros de dados estáticos
TSharedPtr<SRPG_SystemsEditorWindow> FRPG_SystemsEditorModule::RPG_SystemsEditorWindow = nullptr;
TSharedPtr<SWindow> FRPG_SystemsEditorModule::MyWindow = nullptr;

static const FName RPG_EditorWindowTabName("RPG Systems Editor");

void FRPG_SystemsEditorModule::StartupModule()
{
/*	
#pragma region JanelaVazia
	
	//Adicionar janela Vazia 
	// Criando e adicionando a janela standalone ao viewport
	// Criar e configurar a janela
	MyWindow = SNew(SWindow)
		.Title(FText::FromString(TEXT("RPG Systems Editor")))
		.ClientSize(FVector2D(800, 600))
		.SupportsMaximize(true)
		.SupportsMinimize(true);
	RPG_SystemsEditorWindow = SNew(SRPG_SystemsEditorWindow);
	// Adicionar conteúdo à janela
	MyWindow->SetContent(RPG_SystemsEditorWindow.ToSharedRef());

#pragma endregion

	
	
	// Adicionar a janela ao Slate Application
	if (FSlateApplication::IsInitialized())
	{
		//Adiciona conteúdo a uma janela 
		FSlateApplication::Get().AddWindow(MyWindow.ToSharedRef());
		//FSlateApplication::Get().AddWindowAsNativeChild(MyWindow, nullptr);
    
		
	}

	*/
	UE_LOG(LogTemp, Warning, TEXT("RPG_Systems Inicializado "));

	// Register a function to be called when menu system is initialized
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(
		this, &FRPG_SystemsEditorModule::RegisterMenuExtensions));
}

void FRPG_SystemsEditorModule::ShutdownModule()
{
	

	//Tirar o registro do botão da minha barra de tarefas
	// Unregister the startup function
	UToolMenus::UnRegisterStartupCallback(this);
 
	// Unregister all our menu extensions
	UToolMenus::UnregisterOwner(this);

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(RPG_EditorWindowTabName);
}


void FRPG_SystemsEditorModule::CreateRPG_EditorWindow()
{
	/*
	// Criar e configurar a janela
	MyWindow = SNew(SWindow)
		.Title(FText::FromString(TEXT("RPG Systems Editor")))
		.ClientSize(FVector2D(800, 600))
		.SupportsMaximize(true)
		.SupportsMinimize(true);
	RPG_SystemsEditorWindow = SNew(SRPG_SystemsEditorWindow);
	// Adicionar conteúdo à janela
	MyWindow->SetContent(RPG_SystemsEditorWindow.ToSharedRef());

	// Adicionar a janela ao Slate Application
	if (FSlateApplication::IsInitialized())
	{
		//Adiciona conteúdo a uma janela 
		//FSlateApplication::Get().AddWindow(MyWindow.ToSharedRef());
		//FSlateApplication::Get().AddWindowAsNativeChild(MyWindow, nullptr);
    
		
	}
*/
	
	UE_LOG(LogTemp, Warning, TEXT("Tentando criar tab  "));
	
	// Adicionar o dock tab ao dock manager global
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(RPG_EditorWindowTabName, FOnSpawnTab::CreateRaw(this, &FRPG_SystemsEditorModule::OnSpawnPluginTab))
	.SetDisplayName(LOCTEXT("FtestWindowTabTitle", "testWindow"))
	.SetMenuType(ETabSpawnerMenuType::Hidden);
	//Tenta abrir a Tab
	FGlobalTabmanager::Get()->TryInvokeTab(RPG_EditorWindowTabName);

	if (MyWindow)
	{
		//Adiciona uma função da ao evento
		MyWindow->GetOnWindowClosedEvent().AddRaw(this,&FRPG_SystemsEditorModule::OnRPG_EditorWindowClosed);

	}
}

void FRPG_SystemsEditorModule::OnRPG_EditorWindowClosed(const TSharedRef<SWindow>& Window)
{
	if (RPG_SystemsEditorWindow.IsValid())
	{
		// Redefine o ponteiro da variável da janela 
		RPG_SystemsEditorWindow.Reset();
	}
	if (MyWindow.IsValid())
	{
		// Redefine o ponteiro da variável da janela 
		MyWindow.Reset();
	}
}

TSharedRef<SDockTab> FRPG_SystemsEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	RPG_SystemsEditorWindow = SNew(SRPG_SystemsEditorWindow);
	// Criar um dock tab para a janela flutuante
	auto RPGEditorTab = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		.Label(FText::FromString(TEXT("RPG Systems Editor")))
		[
			SNew(SSplitter)
	+ SSplitter::Slot()
	.Value(0.7f) // Define a proporção do tamanho inicial do slot (0.7 para 70%)
	[
		SNew(SButton).Text(FText::FromString("Text")) // Conteúdo do lado esquerdo
	]
	+ SSplitter::Slot()
	[
		RPG_SystemsEditorWindow.ToSharedRef() // Conteúdo do lado direito
	]

			
		];
	UE_LOG(LogTemp, Warning, TEXT("Tentando criar a tab "));
	
	
	return RPGEditorTab;
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FRPG_SystemsEditorModule, RPG_SystemsEditor)