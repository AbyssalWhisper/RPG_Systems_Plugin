// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Blueprint/UserWidget.h"
#include "ConfirmationDialogInterface.h"
#include "ConfirmationDialogAsync.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmationDialogResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmationDialogWidgetCreated, UUserWidget*, DialogWidget);

/**
 * Ação assíncrona que exibe um widget de confirmação e aguarda a resposta do usuário.
 *
 * Como usar no Blueprint:
 *  1. Chame o nó "Show Confirmation Dialog" passando o WorldContext e a classe do seu widget.
 *  2. Conecte os pinos de saída "On Confirmed" e "On Rejected" às ações desejadas.
 *  3. No widget criado, use o pino "Async Task" (proxy) para chamar "Confirm" ou "Reject".
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class RPG_SYSTEMS_API UConfirmationDialogAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	// ─── Pinos de saída (output execution pins) ───────────────────────────────

	/** Disparado quando o usuário confirma a ação */
	UPROPERTY(BlueprintAssignable, Category = "Confirmation Dialog")
	FOnConfirmationDialogResult OnConfirmed;

	/** Disparado quando o usuário recusa / cancela a ação */
	UPROPERTY(BlueprintAssignable, Category = "Confirmation Dialog")
	FOnConfirmationDialogResult OnRejected;

	/** Disparado quando o widget de diálogo é criado e adicionado ao viewport */
	UPROPERTY(BlueprintAssignable, Category = "Confirmation Dialog")
	FOnConfirmationDialogWidgetCreated OnDialogCreated;

	// ─── Nó Blueprint ─────────────────────────────────────────────────────────

	/**
	 * Exibe um widget de confirmação customizável e aguarda a resposta do usuário.
	 * O seletor aceita apenas Widget Blueprints que implementem ConfirmationDialogInterface.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|Confirmation",
		meta = (
			BlueprintInternalUseOnly = "true",
			WorldContext = "WorldContextObject",
			DisplayName = "Show Confirmation Dialog",
			Keywords = "confirm dialog popup async soft class"))
	static UConfirmationDialogAsync* ShowConfirmationDialog(
		UObject* WorldContextObject,
		UPARAM(meta = (MustImplement = "/Script/RPG_Systems.ConfirmationDialogInterface", BlueprintBaseOnly = "true"))
		TSoftClassPtr<UUserWidget> WidgetClass,
		FConfirmationDialogSetup Setup,
		int32 ZOrder = 100);

	// ─── Métodos chamados a partir do Widget ──────────────────────────────────

	/**
	 * Chame este método do seu widget Blueprint para confirmar.
	 * Isso dispara o pino "On Confirmed" e remove o widget da tela.
	 */
	UFUNCTION(BlueprintCallable, Category = "Confirmation Dialog",
		meta = (DisplayName = "Confirm"))
	void Confirm();

	/**
	 * Chame este método do seu widget Blueprint para recusar/cancelar.
	 * Isso dispara o pino "On Rejected" e remove o widget da tela.
	 */
	UFUNCTION(BlueprintCallable, Category = "Confirmation Dialog",
		meta = (DisplayName = "Reject"))
	void Reject();

	/**
	 * Retorna a instância do widget de diálogo criada para esta ação.
	 * Use no widget para acessar o AsyncTask e chamar Confirm/Reject.
	 */
	UFUNCTION(BlueprintPure, Category = "Confirmation Dialog",
		meta = (DisplayName = "Get Dialog Widget"))
	UUserWidget* GetDialogWidget() const { return DialogWidget; }

	// ─── UBlueprintAsyncActionBase ────────────────────────────────────────────

	virtual void Activate() override;

private:

	/** Cria o widget, adiciona ao viewport e guarda referência */
	void CreateAndShowWidget();

	/** Remove o widget da tela e sinaliza a tarefa como completa */
	void CleanUp();

	UPROPERTY()
	TObjectPtr<UObject> CachedWorldContextObject;

	UPROPERTY()
	TSoftClassPtr<UUserWidget> CachedWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> DialogWidget;

	FConfirmationDialogSetup CachedSetup;

	int32 CachedZOrder = 100;

	/** Garante que os delegates só sejam disparados uma vez */
	bool bResultDispatched = false;
};








