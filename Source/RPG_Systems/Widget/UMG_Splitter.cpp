// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_Splitter.h"


// Implementação do SExtendSplitter


FReply SExtendSplitter::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (HoveredHandleIndex + 1 > Children.Num() - 1)
	{
		bIsResizing = false;
	}
	return SSplitter::OnMouseMove(MyGeometry, MouseEvent);
	
}




// Associa o widget Slate ao slot.
void USplitterSlot::BuildSlot(TSharedRef<SWidget> InSlotWidget)
{
	SlotWidget = InSlotWidget;
}

// Libera a referência ao widget Slate associado ao slot.
void USplitterSlot::ReleaseSlot()
{
	SlotWidget.Reset();
}

// Libera recursos Slate do slot, incluindo widgets filhos se necessário.
void USplitterSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	SlotWidget.Reset();
}

// Construtor do UUMG_Splitter, define visibilidade padrão e variável.
UUMG_Splitter::UUMG_Splitter(const FObjectInitializer& ObjectInitializer)
{
	bIsVariable = true;
	SetVisibility(ESlateVisibility::Visible);
}

// Reconstrói o widget Slate associado ao splitter.
TSharedRef<SWidget> UUMG_Splitter::RebuildWidget()
{
	MySplitter = SNew(SExtendSplitter)
		.Orientation(Orientation == ESplitterOrientation::Horizontal ? Orient_Horizontal : Orient_Vertical)
		.ResizeMode(ESplitterResizeMode::FixedPosition)
		.PhysicalSplitterHandleSize(SplitterSize)
		.HitDetectionSplitterHandleSize(FMath::Max(SplitterSize, 5.0f))
		.OnSplitterFinishedResizing_Lambda([this]()
		{
			OnSplitterResized();
		});

	// Adicionar todos os slots filhos
	for (UPanelSlot* PanelSlot : Slots)
	{
		if (UWidget* ChildWidget = PanelSlot->Content)
		{
			USplitterSlot* SplitterSlot = Cast<USplitterSlot>(PanelSlot);
			if (SplitterSlot)
			{
				TSharedRef<SWidget> ChildSlateWidget = ChildWidget->TakeWidget();
                
				// Adicionar slot ao splitter com configuração de tamanho
				MySplitter->AddSlot()
				.Value(SplitterSlot->SizeRatio)
				.Resizable(SplitterSlot->bIsResizable)
				[
					ChildSlateWidget
				];
                
				SplitterSlot->BuildSlot(ChildSlateWidget);
			}
		}
	}

	return MySplitter.ToSharedRef();
}

// Chamado quando um slot é adicionado ao splitter.
void UUMG_Splitter::OnSlotAdded(UPanelSlot* InSlot)
{
	if (MySplitter.IsValid())
	{
		USplitterSlot* SplitterSlot = Cast<USplitterSlot>(InSlot);
		if (SplitterSlot && SplitterSlot->Content)
		{
			TSharedRef<SWidget> ChildSlateWidget = SplitterSlot->Content->TakeWidget();
            
			MySplitter->AddSlot()
			.Value(SplitterSlot->SizeRatio)
			.Resizable(SplitterSlot->bIsResizable)
			[
				ChildSlateWidget
			];
            
			SplitterSlot->BuildSlot(ChildSlateWidget);
		}
	}
}

// Chamado quando um slot é removido do splitter.
void UUMG_Splitter::OnSlotRemoved(UPanelSlot* InSlot)
{
	USplitterSlot* SplitterSlot = Cast<USplitterSlot>(InSlot);
	if (SplitterSlot)
	{
		SplitterSlot->ReleaseSlot();
        
		if (MySplitter.IsValid())
		{
			// Reconstruir o splitter sem este slot
			MySplitter->ClearChildren();
            
			for (UPanelSlot* PanelSlot : Slots)
			{
				if (PanelSlot != InSlot && PanelSlot->Content)
				{
					USplitterSlot* OtherSplitterSlot = Cast<USplitterSlot>(PanelSlot);
					if (OtherSplitterSlot)
					{
						TSharedRef<SWidget> ChildSlateWidget = OtherSplitterSlot->Content->TakeWidget();
                        
						MySplitter->AddSlot()
						.Value(OtherSplitterSlot->SizeRatio)
						.Resizable(OtherSplitterSlot->bIsResizable)
						[
							ChildSlateWidget
						];
                        
						OtherSplitterSlot->BuildSlot(ChildSlateWidget);
					}
				}
			}
		}
	}
}

// Remove todos os widgets filhos do splitter.
void UUMG_Splitter::ClearChildren()
{
	if (MySplitter.IsValid())
	{
		MySplitter->ClearChildren();
		
		
	}
	
	Super::ClearChildren();
}

// Retorna a classe de slot utilizada pelo splitter.
UClass* UUMG_Splitter::GetSlotClass() const
{
	return USplitterSlot::StaticClass();
}

// Libera recursos Slate do splitter e seus filhos.
void UUMG_Splitter::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	if (bReleaseChildren && MySplitter.IsValid())
	{
		MySplitter->ClearChildren();
		MySplitter.Reset();
	}
}

// Adiciona um widget filho ao splitter e retorna o slot criado.
// Atualmente retorna nullptr como placeholder.
USplitterSlot* UUMG_Splitter::AddChild(UWidget* Content)
{
	Super::AddChild(Content);
	return nullptr; // Placeholder, should return a valid USplitterSlot instance
}

// Remove um widget filho do splitter.
bool UUMG_Splitter::RemoveChild(UWidget* Content)
{
	return Super::RemoveChild(Content);
}

void UUMG_Splitter::OnSplitterResized()
{
}
