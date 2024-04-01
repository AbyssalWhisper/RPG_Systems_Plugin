// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/InventorySystem/RPG_InventoryComponent.h"
#include "RPG_Systems/PlayerController/RPG_PlayerController.h"
#include "RPG_Systems/InteractiveObjects/RPG_InteractComponent.h"
#include "RPG_Systems/Character/RPG_BaseCharacter.h"
#include "RPG_Systems/BlueprintLibrary/RPG_BP_Library_Utilities.h"

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
	Items.SetNum(Slots);
}


bool URPG_InventoryComponent::TryAddItem(FSTR_RPG_ItemSlot Item, FSTR_RPG_ItemSlot& RemainingItems)
{
	
	int EmptySlotIndex = -1;
	RemainingItems = Item;
	if (Item.Count < 1 || Item.ItemID == "")return false;

	if (!Item_DT.DataTable) {
		GEngine->AddOnScreenDebugMessage(-1, 25, FColor::Red, "Item DT Invalid");
		return false;
	}

	FSTR_RPG_ItemData* ItemData = Item_DT.DataTable->FindRow<FSTR_RPG_ItemData>(Item.ItemID, "");
	
    if(!ItemData) return false;
	
	SearchSlotsWithItem(Item, ItemData, RemainingItems);
	
	
	if (RemainingItems.Count > 0)
	{
		SearchEmptySlots(RemainingItems, ItemData, RemainingItems);

	}

	return true;
}
void URPG_InventoryComponent::RemoveItem(FName ItemID, int Count)
{
	int InventorySize = Items.Num();
	Count = Count < 1 ? 1 : Count;
	for (int i = 0; i < InventorySize; i++)
	{
		if (Items[i].ItemID == ItemID)
		{
			if (Items[i].Count - Count < 1)
			{
				Items[i] = FSTR_RPG_ItemSlot();
			}
			else
			{
				Items[i].Count -= Count;
			}
			break;
		}
	}
	
}

void URPG_InventoryComponent::RemoveItemFromIndex(int index, int Count, bool& Sucess)
{
	Sucess = false;
	if (Items.IsValidIndex(index) && Items[index].Count > 0)
	{
		Items[index].Count -= Count;
		if (Items[index].Count <= 0)
		{
			Items[index] = FSTR_RPG_ItemSlot();
		}
		Sucess = true;
		UpdatePlayersSlot(index);
	}
}

void URPG_InventoryComponent::SearchSlotsWithItem(FSTR_RPG_ItemSlot Item, FSTR_RPG_ItemData* ItemData, FSTR_RPG_ItemSlot& RemainingItems)
{
	int InventorySize = Items.Num();

	RemainingItems = Item;

	for (int i = 0; i < InventorySize; i++)
	{
		if (Items[i].ItemID == Item.ItemID)
		{
			if (Items[i].Count < FMath::Max(ItemData->MaxCount, 1))
			{
				if ((Items[i].Count + Item.Count) > FMath::Max(ItemData->MaxCount,1)) {

					Item.Count -= FMath::Max(ItemData->MaxCount, 1) - Items[i].Count;
					Items[i].Count = FMath::Max(ItemData->MaxCount, 1);
					RemainingItems = Item;
					UpdatePlayersSlot(i);
					
				}
				else
				{
					Items[i].Count += Item.Count;
					RemainingItems = FSTR_RPG_ItemSlot();
					UpdatePlayersSlot(i);
					
					break;
				}
			}
			
		}

		
	}
}

void URPG_InventoryComponent::SearchEmptySlots(FSTR_RPG_ItemSlot Item, FSTR_RPG_ItemData* ItemData, FSTR_RPG_ItemSlot& RemainingItems)
{
	int InventorySize = Items.Num();
	for (int i = 0; i < InventorySize; i++)
	{
		if (Items[i].ItemID == "")
		{
			if (Item.Count > FMath::Max(ItemData->MaxCount, 1)) {
				
				Item.Count -= FMath::Max(ItemData->MaxCount, 1);
				Items[i].ItemID = Item.ItemID;
				Items[i].Count = FMath::Max(ItemData->MaxCount, 1);
				 
				RemainingItems = Item;
				UpdatePlayersSlot(i);
				
			}
			else
			{
				Items[i].ItemID = Item.ItemID;
				Items[i].Count = Item.Count;
				 
				RemainingItems = FSTR_RPG_ItemSlot();
				UpdatePlayersSlot(i);
				
				break;
			}
		}
	}
}

void URPG_InventoryComponent::AddItemAtIndex(FSTR_RPG_ItemSlot Item, int SlotIndex, FSTR_RPG_ItemData* ItemData, FSTR_RPG_ItemSlot& RemainingItems)
{
	RemainingItems = Item;

	if (RemainingItems.ItemID == "")return;
	 
	if (Items[SlotIndex].ItemID == RemainingItems.ItemID)
	{
	
		if (Items[SlotIndex].Count + RemainingItems.Count > ItemData->MaxCount)
		{
			 
			RemainingItems.Count -= ItemData->MaxCount - Items[SlotIndex].Count;
			Items[SlotIndex].Count = ItemData->MaxCount;
			UpdatePlayersSlot(SlotIndex);
		}
		else
		{
			Items[SlotIndex].Count += RemainingItems.Count;
			RemainingItems = FSTR_RPG_ItemSlot();
			UpdatePlayersSlot(SlotIndex);
		}
	}
	else
	{
		if (Items[SlotIndex].ItemID == "")
		{
			Items[SlotIndex].ItemID = RemainingItems.ItemID;
			if (RemainingItems.Count > ItemData->MaxCount)
			{
				Items[SlotIndex].Count = ItemData->MaxCount;
				RemainingItems.Count -= ItemData->MaxCount;
				UpdatePlayersSlot(SlotIndex);
			}
			else
			{
				Items[SlotIndex].Count = RemainingItems.Count;
				RemainingItems = FSTR_RPG_ItemSlot();
				UpdatePlayersSlot(SlotIndex);
			}
		}
	}
}

void URPG_InventoryComponent::UpdatePlayersSlot(int SlotIndex)
{
	
	int Size = Players.Num();
	for (int i = 0; i < Size; i++)
	{
		if (Players[i])
		{

			Players[i]->InteractionManager->Client_UpdateInventorySlot(this, this->Items[SlotIndex], SlotIndex);
			
		}
	}

}

void URPG_InventoryComponent::SetItemSlot(FSTR_RPG_ItemSlot Item, int SlotIndex)
{
 
		Items[SlotIndex] = Item;
		OnInventorySlotChange.Broadcast(Items[SlotIndex], SlotIndex);		
 
}

void URPG_InventoryComponent::AddPlayer(ARPG_PlayerController* PlayerController)
{
	if (PlayerController)
	{
		Players.AddUnique(PlayerController);
	}
}

void URPG_InventoryComponent::RemovePlayer(ARPG_PlayerController* PlayerController)
{
	if (PlayerController)
	{
		int index = Players.Find(PlayerController);
		if (Players.Find(PlayerController) > -1)
		{
			Players.Remove(PlayerController);
		}
		
	}
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
		FSTR_RPG_ItemData* Item1Data = Item_DT.DataTable->FindRow<FSTR_RPG_ItemData>(Container1->Items[SlotIndex].ItemID, "", true);
		FSTR_RPG_ItemData* Item2Data = Item_DT.DataTable->FindRow<FSTR_RPG_ItemData>(Container2->Items[TargetSlotIndex].ItemID, "", true);
		
		if (Container1->Items[SlotIndex].ItemID == Container2->Items[TargetSlotIndex].ItemID)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, Container1->Items[SlotIndex].ItemID.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, Container2->Items[TargetSlotIndex].ItemID.ToString());
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
				if (Container2->Items[TargetSlotIndex].ItemID == "")
				{
					Container2->Items[TargetSlotIndex].ItemID = Item1.ItemID;
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
		FSTR_RPG_ItemData ItemData = GetItemData(Items[FMath::Min(SlotIndex, Items.Num() - 1)]);
		if (ItemData.Tags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Item.Usable"))))
		{
			if (ItemData.Ability)
			{
				FGameplayEventData GameplayEventData;
				FRPG_GameplayAbilityTargetData_Item* Data = new FRPG_GameplayAbilityTargetData_Item(SlotIndex);
				FGameplayAbilityTargetDataHandle Handle;
				Handle.Data.Add(TSharedPtr<FGameplayAbilityTargetData>(Data));
				if (!Data)return;
				FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName("Event.ReceiveItemData"));
				if (!Tag.IsValid())return;
				GameplayEventData.TargetData = Handle;
				GameplayEventData.OptionalObject = this;
				GameplayEventData.Instigator = OwnerCharacter;
				if (OwnerCharacter->GetAbilitySystemComponent()->TryActivateAbilityByClass(ItemData.Ability))
				{
					OwnerCharacter->GetAbilitySystemComponent()->HandleGameplayEvent(Tag,&GameplayEventData);
				}
				
			}
		}
	}
}

FSTR_RPG_ItemData URPG_InventoryComponent::GetItemDataFromSlot(int SlotIndex)
{
	return GetItemData(Items[SlotIndex]);
}

FSTR_RPG_ItemData URPG_InventoryComponent::GetItemData(FSTR_RPG_ItemSlot Item)
{
	FSTR_RPG_ItemData* Item1Data = Item_DT.DataTable->FindRow<FSTR_RPG_ItemData>(Item.ItemID, "", true);
	if (Item1Data)
	{
		return *Item1Data;
	}
	return FSTR_RPG_ItemData();
}

 
