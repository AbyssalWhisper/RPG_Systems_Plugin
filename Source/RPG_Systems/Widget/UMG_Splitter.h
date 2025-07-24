// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Define para tornar membros privados públicos temporariamente
#include "Components/PanelWidget.h"
#include "Widgets/Layout/SSplitter.h"

#include "UMG_Splitter.generated.h"

/**
 * 
 */

class SExtendSplitter : public SSplitter
{
public:
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
	
};

UENUM(BlueprintType)
enum class ESplitterOrientation : uint8
{
	Horizontal UMETA(DisplayName = "Horizontal"),
	Vertical UMETA(DisplayName = "Vertical")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSplitterMoved, float, NewRatio);


UCLASS()
class RPG_SYSTEMS_API USplitterSlot : public UPanelSlot
{
	GENERATED_BODY()

public:
	/** Proporção do espaço que este slot ocupa (0.0 a 1.0) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SizeRatio = 0.5f;

	/** Se este slot pode ser redimensionado */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	bool bIsResizable = true;

public:
	/** Associa o widget Slate ao slot */
	void BuildSlot(TSharedRef<SWidget> InSlotWidget);

	/** Libera a referência ao widget Slate associado ao slot */
	void ReleaseSlot();
    
	/** Libera recursos Slate do slot, incluindo widgets filhos se necessário */
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:
	TSharedPtr<SWidget> SlotWidget;
};

UCLASS(blueprintType, Blueprintable)
class RPG_SYSTEMS_API UUMG_Splitter : public UPanelWidget
{
public:
	GENERATED_BODY()

	/** Orientação do splitter (Horizontal ou Vertical) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Splitter")
	ESplitterOrientation Orientation = ESplitterOrientation::Horizontal;

	/** Largura/altura da barra divisória em pixels */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Splitter")
	float SplitterSize = 8.0f;
    
	/** Evento disparado quando o splitter é movido */
	UPROPERTY(BlueprintAssignable, Category = "Splitter")
	FOnSplitterMoved OnSplitterMoved;

	
	/** Construtor do splitter */
	UUMG_Splitter(const FObjectInitializer& ObjectInitializer);

	/** Reconstrói o widget Slate associado ao splitter */
	virtual TSharedRef<SWidget> RebuildWidget() override;

	/** Chamado quando um slot é adicionado ao splitter */
	virtual void OnSlotAdded(UPanelSlot* InSlot) override;

	/** Chamado quando um slot é removido do splitter */
	virtual void OnSlotRemoved(UPanelSlot* InSlot) override;

	/** Remove todos os widgets filhos do splitter */
	virtual void ClearChildren() override;

	/** Retorna a classe de slot utilizada pelo splitter */
	virtual UClass* GetSlotClass() const override;

	/** Libera recursos Slate do splitter e seus filhos */
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	/** Adiciona um widget filho ao splitter e retorna o slot criado */
	USplitterSlot* AddChild(UWidget* Content);

	/** Remove um widget filho do splitter */
	bool RemoveChild(UWidget* Content);

protected:
	/** Referência para o widget nativo do Slate */
	TSharedPtr<class SExtendSplitter> MySplitter;

	/** Callback chamado quando o splitter é redimensionado */
	void OnSplitterResized();


};
