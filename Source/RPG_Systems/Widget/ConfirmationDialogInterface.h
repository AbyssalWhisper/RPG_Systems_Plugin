// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ConfirmationDialogInterface.generated.h"

/** Dados de customização passados ao widget de confirmação ao ser criado */
USTRUCT(BlueprintType)
struct FConfirmationDialogSetup
{
	GENERATED_BODY()

	/** Título exibido no topo do diálogo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Confirmation Dialog")
	FText Title = FText::FromString(TEXT("Confirmação"));

	/** Mensagem/descrição do diálogo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Confirmation Dialog")
	FText Message = FText::FromString(TEXT("Deseja confirmar esta ação?"));

	/** Texto do botão de confirmar */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Confirmation Dialog")
	FText ConfirmButtonText = FText::FromString(TEXT("Confirmar"));

	/** Texto do botão de rejeitar/cancelar */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Confirmation Dialog")
	FText RejectButtonText = FText::FromString(TEXT("Cancelar"));

	/** Dados extras genéricos (ex: nome do item, quantidade, etc.) para uso livre no widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Confirmation Dialog")
	TMap<FString, FString> ExtraData;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UConfirmationDialogInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Implemente esta interface no seu Widget Blueprint de confirmação.
 * O sistema chamará "Setup Confirmation Dialog" automaticamente ao criar o widget,
 * passando todos os dados de customização e a referência ao AsyncTask.
 */
class RPG_SYSTEMS_API IConfirmationDialogInterface
{
	GENERATED_BODY()

public:

	/**
	 * Chamado automaticamente ao criar o widget.
	 * Use para popular textos, imagens e qualquer dado visual do diálogo.
	 *
	 * @param Setup      Dados de customização (título, mensagem, textos dos botões, extras)
	 * @param AsyncTask  Referência ao nó assíncrono — chame Confirm() ou Reject() nele
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Confirmation Dialog")
	void SetupConfirmationDialog(
		const FConfirmationDialogSetup& Setup,
		UObject* AsyncTask);
};

