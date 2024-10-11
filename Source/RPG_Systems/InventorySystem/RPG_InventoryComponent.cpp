// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/InventorySystem/RPG_InventoryComponent.h"

#include "BetterUtilitiesBPLibrary.h"
#include "RPG_Systems/PlayerController/RPG_PlayerController.h"
#include "RPG_Systems/InteractiveObjects/RPG_InteractComponent.h"
#include "RPG_Systems/Character/RPG_BaseCharacter.h"
#include "RPG_Systems/BlueprintLibrary/RPG_BP_Library_Utilities.h"
#include "RPG_Systems/InventorySystem/RPG_ItemData.h"
#include "RPG_Systems/InventorySystem/ItemUseCondition/RPG_UseItemConditionComponent.h"
#include "RPG_Systems/InventorySystem/ItemExecuteCode/RPG_ExecuteItemCodeComponent.h"
#include "RPG_Systems/InventorySystem/ItemExecuteCode/RPG_AddedOrRemovedItemCodeComponent.h"



// Sets default values for this component's properties
URPG_InventoryComponent::URPG_InventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	/*
	static ConstructorHelpers::FObjectFinder<UDataTable> DatatableRef(TEXT("DataTable'/Game/Blueprints/InventorySystem/DT_Items__.DT_Items__'"));
	if (DatatableRef.Succeeded())
	{
		Item_DT.DataTable = DatatableRef.Object;
	}*/

	
}


// Called when the game starts
void URPG_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InitContainer();
	
		OwnerCharacter = Cast<ARPG_BaseCharacter>(GetOwner());
	// ...
	
}


// Called every frame
void URPG_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URPG_InventoryComponent::InitContainer()
{
	if(Items.Num() != Slots) Items.SetNum(Slots);
}

void URPG_InventoryComponent::Client_InitContainer_Implementation()
{
	InitContainer();
}

void URPG_InventoryComponent::Multicast_InitContainer_Implementation()
{
	InitContainer();
}


bool URPG_InventoryComponent::TryAddItem(URPG_ItemData* ItemData,int Count, FSTR_RPG_ItemSlot& RemainingItems)
{
	FSTR_RPG_ItemSlot ItemSlot_ = FSTR_RPG_ItemSlot(ItemData,Count);
	int EmptySlotIndex = -1;
	RemainingItems = ItemSlot_;
	if (ItemSlot_.Count < 1 || !ItemSlot_.Item)return false;
	
	SearchSlotsWithItem(ItemSlot_, RemainingItems);
	
	
	if (RemainingItems.Count > 0)
	{
		SearchEmptySlots(RemainingItems, RemainingItems);
	}

	return true;
}
void URPG_InventoryComponent::RemoveItem(URPG_ItemData* Item_, int Count)
{
	int InventorySize = GetInventorySize();
	Count = Count < 1 ? 1 : Count;
	for (int i = 0; i < InventorySize; i++)
	{
		if (Items[i].Item == Item_)
		{
			if (Items[i].Count <= Count)
			{
				Count -= Items[i].Count;
				OnItemRemoved(Item_, Items[i].Count);
				Items[i] = FSTR_RPG_ItemSlot();
				
			}
			else
			{
				Items[i].Count -= Count;
				OnItemRemoved(Item_, Count);
				Count = 0;
			}
			UpdatePlayersSlot(i);
			if (Count <= 0)break;
		}
	}
	
}

void URPG_InventoryComponent::RemoveItemFromIndex(int SlotIndex, int Count, bool& Sucess)
{
	Sucess = false;
	if (Items.IsValidIndex(SlotIndex) && Items[SlotIndex].Count > 0)
	{
		int LastItemCount = Items[SlotIndex].Count;

		Items[SlotIndex].Count -= Count;
		if (Items[SlotIndex].Count <= 0)
		{
			OnItemRemoved(Items[SlotIndex].Item, Count);
			Items[SlotIndex] = FSTR_RPG_ItemSlot();
			
		} else {
			OnItemRemoved(Items[SlotIndex].Item, LastItemCount);
		}
		Sucess = true;
		UpdatePlayersSlot(SlotIndex);
	}
	
}

void URPG_InventoryComponent::SearchSlotsWithItem(FSTR_RPG_ItemSlot Item, FSTR_RPG_ItemSlot& RemainingItems)
{
	const int InventorySize = GetInventorySize();
	const int ItemMaxCount = FMath::Max(Item.Item->MaxCount, 1);

	//RemainingItems = Item;
	for (int i = 0; i < InventorySize; i++)
	{
		if (Items[i].Item == Item.Item)
		{
			if (Items[i].Count < FMath::Max(Item.Item->MaxCount, 1))
			{
				if ((Items[i].Count + Item.Count) > ItemMaxCount) {

					const int CountToAdd = ItemMaxCount - Items[i].Count;
					OnItemAdded(Item.Item, CountToAdd);

					Item.Count -= CountToAdd;
					Items[i].Count = ItemMaxCount;

					RemainingItems = Item;
					UpdatePlayersSlot(i);
					
				}
				else
				{
					Items[i].Count += Item.Count;
					OnItemAdded(Item.Item, Item.Count);
					RemainingItems = FSTR_RPG_ItemSlot();
					UpdatePlayersSlot(i);
					
					break;
				}
			}
			
		}

		
	}

	
}

void URPG_InventoryComponent::SearchEmptySlots(FSTR_RPG_ItemSlot Item,FSTR_RPG_ItemSlot& RemainingItems)
{
	const int InventorySize = GetInventorySize();
	const int MaxItemPerSlot = FMath::Max(Item.Item->MaxCount, 1) ;
	for (int i = 0; i < InventorySize; i++)
	{
		if (Items[i].Item == nullptr)
		{
			if (Item.Count > MaxItemPerSlot) {
				
				Item.Count -= MaxItemPerSlot;
				Items[i].Item = Item.Item;
				Items[i].Count = MaxItemPerSlot;
				
				RemainingItems = Item;

				OnItemAdded(Item.Item, MaxItemPerSlot);
				UpdatePlayersSlot(i);
				
			}
			else
			{
				Items[i].Item = Item.Item;
				Items[i].Count = Item.Count;
				
				RemainingItems = FSTR_RPG_ItemSlot();

				OnItemAdded(Item.Item, Item.Count);
				UpdatePlayersSlot(i);
				
				break;
			}
		}
	}
}

void URPG_InventoryComponent::AddItemAtIndex(FSTR_RPG_ItemSlot Item, int SlotIndex, FSTR_RPG_ItemSlot& RemainingItems)
{
	if (!Item.Item && Item.Count <= 0)return;

	RemainingItems = Item;

	const int MaxItemPerSlot = Item.Item->MaxCount;
	
	if (RemainingItems.Item == nullptr)return;
	 
	if (Items[SlotIndex].Item == RemainingItems.Item)
	{
	
		if (Items[SlotIndex].Count + RemainingItems.Count > MaxItemPerSlot)
		{
			 
			RemainingItems.Count -= MaxItemPerSlot - Items[SlotIndex].Count;
			Items[SlotIndex].Count = MaxItemPerSlot;
		}
		else
		{
			Items[SlotIndex].Count += RemainingItems.Count;
			RemainingItems = FSTR_RPG_ItemSlot();
		}
		UpdatePlayersSlot(SlotIndex);
	}
	else
	{
		if (Items[SlotIndex].Item == nullptr)
		{
			Items[SlotIndex].Item = RemainingItems.Item;
			if (RemainingItems.Count > MaxItemPerSlot)
			{
				Items[SlotIndex].Count = MaxItemPerSlot;
				RemainingItems.Count -= MaxItemPerSlot;
			}
			else
			{
				Items[SlotIndex].Count = RemainingItems.Count;
				RemainingItems = FSTR_RPG_ItemSlot();
			}
			UpdatePlayersSlot(SlotIndex);
		}
	}
	if (RemainingItems.Count <= 0) {
		OnItemAdded(Item.Item, Item.Count);
	}
	else if(RemainingItems.Count < Item.Count)
	{
		OnItemAdded(Item.Item, Item.Count - RemainingItems.Count);
	}
}

void URPG_InventoryComponent::UpdatePlayersSlot(int SlotIndex)
{
	
	int Size = Players.Num();
	for (int i = 0; i < Size; i++)
	{
		if (Players[i])
		{

			URPG_InteractComponent* component = Players[i]->FindComponentByClass<URPG_InteractComponent>();
			if (component)
			{
				component->Client_UpdateInventorySlot(this, this->Items[SlotIndex], SlotIndex);
		
			}
		}
	}
	
	OnInventorySlotChange.Broadcast(Items[SlotIndex], SlotIndex);		
}

void URPG_InventoryComponent::SetItemSlot(FSTR_RPG_ItemSlot Item, int SlotIndex)
{
 
	Items[SlotIndex] = Item;
	UpdatePlayersSlot(SlotIndex);
 
}

void URPG_InventoryComponent::SetItemSlotOnClient(FSTR_RPG_ItemSlot Item, int SlotIndex)
{
	Items[SlotIndex] = Item;
	OnInventorySlotChange.Broadcast(Items[SlotIndex], SlotIndex);	
}

void URPG_InventoryComponent::AddPlayer(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		Players.AddUnique(PlayerController);
		URPG_InteractComponent* a = PlayerController->FindComponentByClass<URPG_InteractComponent>();
		if (a) a->InitContainerInventory(this);
		UpdateAllInventory(PlayerController);
	}
}

void URPG_InventoryComponent::RemovePlayer(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		const int index = Players.Find(PlayerController);
		if (index > -1)
		{
			Players.Remove(PlayerController);
		}
	}
}

bool URPG_InventoryComponent::HasItem(URPG_ItemData* Item){

	const int InventorySize = GetInventorySize();
	for (int i = 0; i < InventorySize; i++)
	{
		if (Items[i].Item == Item)
		{
			return true;
		}
	}
	return false;
}

bool URPG_InventoryComponent::HasItemCount(URPG_ItemData* Item, int Count)
{
	const int InventorySize = GetInventorySize();
	Count = Count < 1 ? 1 : Count;
	int FoundCount = 0;
	for (int i = 0; i < InventorySize; i++)
	{
		if (Items[i].Item == Item)
		{
			FoundCount += Items[i].Count;
			if (FoundCount >= Count)return true;
		}
	}
	return false;
}

bool URPG_InventoryComponent::isInventoryFullOfItems()
{
	const int InventorySize = GetInventorySize();
	for (int i = 0; i < InventorySize; i++)
	{
		if (Items[i].Item == nullptr)
		{
			return false;
		}
	}
	return true;
}

int URPG_InventoryComponent::GetItemCount(URPG_ItemData* Item)
{
	const int InventorySize = GetInventorySize();
	int FoundCount = 0;
	for (int i = 0; i < InventorySize; i++)
	{
		if (Items[i].Item == Item)
		{
			FoundCount += Items[i].Count;
		}
	}
	return FoundCount;
}


void URPG_InventoryComponent::TransferItem(int SlotIndex, URPG_InventoryComponent* Container1,int Amount, int TargetSlotIndex, URPG_InventoryComponent* Container2)
{
	FSTR_RPG_ItemSlot  Item1;
	FSTR_RPG_ItemSlot  Item2;
	Amount = FMath::Max(Amount, 1);
	if (Container1 && Container2)
	{
		if (!Item_DT.DataTable)return;
		 
		Item1 = Container1->Items[SlotIndex];
		Item2 = Container2->Items[TargetSlotIndex];
		//FSTR_RPG_ItemData* Item1Data = Item_DT.DataTable->FindRow<FSTR_RPG_ItemData>(Container1->Items[SlotIndex].Item, "", true);
		//FSTR_RPG_ItemData* Item2Data = Item_DT.DataTable->FindRow<FSTR_RPG_ItemData>(Container2->Items[TargetSlotIndex].Item, "", true);

		URPG_ItemData* Item1Data = Container1->Items[SlotIndex].Item;
		
		if (Container1->Items[SlotIndex].Item == Container2->Items[TargetSlotIndex].Item)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, Container1->Items[SlotIndex].Item->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, Container2->Items[TargetSlotIndex].Item->GetName());
			// Se a quantia para movimentar for maior do que a quantia no slot cancelar 
			if (Amount > Container1->Items[SlotIndex].Count)return;
			if (Container2->Items[TargetSlotIndex].Count >= Item1Data->MaxCount)return;
			
			if (Amount + Container2->Items[TargetSlotIndex].Count > Item1Data->MaxCount)
			{
				int MaxAddCount = Item1Data->MaxCount - Container2->Items[TargetSlotIndex].Count;
				if (Amount > MaxAddCount)
				{
					Container1->Items[SlotIndex].Count -= MaxAddCount;
					Container2->Items[TargetSlotIndex].Count = Item1Data->MaxCount;
				}
				else
				{
					Container1->Items[SlotIndex].Count -= Amount;
					Container2->Items[TargetSlotIndex].Count += Amount;
				}
				

				if (Container1->Items[SlotIndex].Count < 1)
				{
					Container1->Items[SlotIndex] = FSTR_RPG_ItemSlot();
				}

				Container1->UpdatePlayersSlot(SlotIndex);
				Container2->UpdatePlayersSlot(TargetSlotIndex);
			}
			else
			{

				Container2->Items[TargetSlotIndex].Count += Amount;
				Container1->Items[SlotIndex].Count -= Amount;
				if (Container1->Items[SlotIndex].Count < 1)
				{
					Container1->Items[SlotIndex] = FSTR_RPG_ItemSlot();
				}
				Container1->UpdatePlayersSlot(SlotIndex);
				Container2->UpdatePlayersSlot(TargetSlotIndex);
			}
		}
		else
		{
			
			if (Amount >= Container1->Items[SlotIndex].Count)
			{
				Container1->Items[SlotIndex] = Item2;
				Container2->Items[TargetSlotIndex] = Item1;

				Container1->UpdatePlayersSlot(SlotIndex);
				Container2->UpdatePlayersSlot(TargetSlotIndex);
			}
			else
			{
				if (Container2->Items[TargetSlotIndex].Item == nullptr)
				{
					Container2->Items[TargetSlotIndex].Item = Item1.Item;
					Container2->Items[TargetSlotIndex].Count = Amount;
					Container1->Items[SlotIndex].Count -= Amount;
					
					Container1->UpdatePlayersSlot(SlotIndex);
					Container2->UpdatePlayersSlot(TargetSlotIndex);
				}
			}	
		}
	}	
}

void URPG_InventoryComponent::TryUseItem(int SlotIndex)
{
	if (OwnerCharacter)
	{
		URPG_ItemData* ItemData = Items[FMath::Min(SlotIndex, Items.Num() - 1)].Item;
		if (!ItemData || !ItemData->ItemType)return;
		if (Items[SlotIndex].Count > 0)
		{
			if (!CanUseItem(SlotIndex))return;
			if(GetOwner()->HasAuthority())
			{
				for (auto ItemCodeComp : ItemData->OnUseItem) {
					ItemCodeComp->ExecuteOnServer(GetOwner(), this, SlotIndex);

				}
			}
			else
			{
				TryUseItem_Server(SlotIndex);
				for (auto ItemCodeComp : ItemData->OnUseItem) {
					ItemCodeComp->ExecuteOnClient(GetOwner(), this, SlotIndex);
				}
			}
		}
	}
}

void URPG_InventoryComponent::TryUseItem_Server_Implementation(int SlotIndex)
{
	TryUseItem(SlotIndex);
			
}

void URPG_InventoryComponent::TryUseItem_Client_Implementation(int SlotIndex)
{
}

URPG_ItemData* URPG_InventoryComponent::GetItemDataFromSlot(int SlotIndex)
{
	return Items[SlotIndex].Item;
}

int URPG_InventoryComponent::GetInventorySize() const
{
	return Items.Num();
}

void URPG_InventoryComponent::UpdateAllInventory(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		
		auto InteractComponent = PlayerController->FindComponentByClass<URPG_InteractComponent>();
		
		if (InteractComponent)
		{
			for (int i = 0; i < Items.Num(); i++)
			{
				InteractComponent->Client_UpdateInventorySlot(this, Items[i], i);
			}
		}
	}
}

bool URPG_InventoryComponent::CanUseItem(int SlotIndex)
{
	if (Items[SlotIndex].Item)
	{
		for (auto& Element : Items[SlotIndex].Item->UseItemConditions)
		{
			if (Element)
			{
				if (!Element->CanUse(GetOwner()))
				{
					return false;
				}
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}

void URPG_InventoryComponent::OnItemAdded(URPG_ItemData* Item_, int Count_)
{
	if (Item_) {
		for (auto Code : Item_->OnItemAddedToInventory) {
			if (Code)
			{
				Code->Execute(this->GetOwner(), this, Item_, Count_, ESlotOp::ItemAdded);
			}
		}
	}
}

void URPG_InventoryComponent::OnItemRemoved(URPG_ItemData* Item_, int Count_)
{
	if (Item_) {
		for (auto Code : Item_->OnItemRemovedFromInventory) {
			if (Code)
			{
				Code->Execute(this->GetOwner(), this, Item_, Count_, ESlotOp::ItemRemoved);
			}
		}
	}
}

