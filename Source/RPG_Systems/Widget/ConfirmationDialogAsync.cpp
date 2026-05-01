// Fill out your copyright notice in the Description page of Project Settings.

#include "ConfirmationDialogAsync.h"
#include "ConfirmationDialogInterface.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "GameFramework/PlayerController.h"

// ─────────────────────────────────────────────────────────────────────────────
// Static factory
// ─────────────────────────────────────────────────────────────────────────────

UConfirmationDialogAsync* UConfirmationDialogAsync::ShowConfirmationDialog(
	UObject* WorldContextObject,
	TSoftClassPtr<UUserWidget> WidgetClass,
	FConfirmationDialogSetup Setup,
	int32 ZOrder)
{
	UConfirmationDialogAsync* Action = NewObject<UConfirmationDialogAsync>();
	Action->CachedWorldContextObject = WorldContextObject;
	Action->CachedWidgetClass        = WidgetClass;
	Action->CachedSetup              = MoveTemp(Setup);
	Action->CachedZOrder             = ZOrder;
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

// ─────────────────────────────────────────────────────────────────────────────
// Activate
// ─────────────────────────────────────────────────────────────────────────────

void UConfirmationDialogAsync::Activate()
{
	if (CachedWidgetClass.IsNull())
	{
		UE_LOG(LogTemp, Warning,
			TEXT("UConfirmationDialogAsync: WidgetClass is null — rejecting immediately."));
		Reject();
		return;
	}

	CreateAndShowWidget();
}

// ─────────────────────────────────────────────────────────────────────────────
// Create & show
// ─────────────────────────────────────────────────────────────────────────────

void UConfirmationDialogAsync::CreateAndShowWidget()
{
	// Obter o PlayerController a partir do WorldContext
	APlayerController* PC = nullptr;

	if (UWorld* World = GEngine->GetWorldFromContextObject(
			CachedWorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		PC = World->GetFirstPlayerController();
	}

	if (!PC)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("UConfirmationDialogAsync: Could not find a PlayerController — rejecting immediately."));
		Reject();
		return;
	}

	UClass* ResolvedWidgetClass = CachedWidgetClass.LoadSynchronous();
	if (!ResolvedWidgetClass || !ResolvedWidgetClass->IsChildOf(UUserWidget::StaticClass()))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("UConfirmationDialogAsync: Failed to load a valid widget class — rejecting immediately."));
		Reject();
		return;
	}

	if (!ResolvedWidgetClass->ImplementsInterface(UConfirmationDialogInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("UConfirmationDialogAsync: Widget class must implement UConfirmationDialogInterface — rejecting immediately."));
		Reject();
		return;
	}

	DialogWidget = CreateWidget<UUserWidget>(PC, ResolvedWidgetClass);
	if (!DialogWidget)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("UConfirmationDialogAsync: Failed to create widget — rejecting immediately."));
		Reject();
		return;
	}

	DialogWidget->AddToViewport(CachedZOrder);

	UUserWidget* CreatedDialogWidget = DialogWidget;
	IConfirmationDialogInterface::Execute_SetupConfirmationDialog(CreatedDialogWidget, CachedSetup, this);

	if (!bResultDispatched && IsValid(CreatedDialogWidget))
	{
		OnDialogCreated.Broadcast(CreatedDialogWidget);
	}
}

// ─────────────────────────────────────────────────────────────────────────────
// Confirm / Reject
// ─────────────────────────────────────────────────────────────────────────────

void UConfirmationDialogAsync::Confirm()
{
	if (bResultDispatched) return;
	bResultDispatched = true;

	OnConfirmed.Broadcast();
	CleanUp();
}

void UConfirmationDialogAsync::Reject()
{
	if (bResultDispatched) return;
	bResultDispatched = true;

	OnRejected.Broadcast();
	CleanUp();
}

// ─────────────────────────────────────────────────────────────────────────────
// Clean up
// ─────────────────────────────────────────────────────────────────────────────

void UConfirmationDialogAsync::CleanUp()
{
	if (DialogWidget)
	{
		DialogWidget->RemoveFromParent();
		DialogWidget = nullptr;
	}

	SetReadyToDestroy();
}








