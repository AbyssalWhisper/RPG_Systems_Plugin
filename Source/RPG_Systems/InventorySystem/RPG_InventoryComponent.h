// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "RPG_Systems/InventorySystem/RPG_ItemStruct.h"
#include "RPG_InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventorySlotChange, FSTR_RPG_ItemSlot, Item, int, Slot);

class ARPG_PlayerController;
class ARPG_BaseCharacter;
UCLASS(Blueprintable, BlueprintType,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_SYSTEMS_API URPG_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPG_InventoryComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle Item_DT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Slots = 5;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FSTR_RPG_ItemSlot> Items;
	UPROPERTY(BlueprintAssignable)
		FOnInventorySlotChange OnInventorySlotChange;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<APlayerController*> Players;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ARPG_BaseCharacter* OwnerCharacter;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	virtual void InitContainer();
	UFUNCTION(Client,Reliable)
	virtual void Client_InitContainer();
	UFUNCTION(NetMulticast,Reliable)
	virtual void Multicast_InitContainer();
	
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly)
	bool TryAddItem(URPG_ItemData* ItemData,int Count, FSTR_RPG_ItemSlot& RemainingItems);
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly)
	void RemoveItem(URPG_ItemData* Item_, int Count);
	void AddItemAtIndex(FSTR_RPG_ItemSlot Item, int SlotIndex, FSTR_RPG_ItemSlot& RemainingItems);
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly)
		void RemoveItemFromIndex(int SlotIndex, int Count,bool& Sucess);
	void SearchSlotsWithItem(FSTR_RPG_ItemSlot Item, FSTR_RPG_ItemSlot& RemainingItems);
	void SearchEmptySlots(FSTR_RPG_ItemSlot Item, FSTR_RPG_ItemSlot& RemainingItems);

	//void UpdateAllPlayersSlots();
	void UpdatePlayersSlot(int SlotIndex);

	
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly)
	void SetItemSlot(FSTR_RPG_ItemSlot Item, int SlotIndex);
	void SetItemSlotOnClient(FSTR_RPG_ItemSlot Item, int SlotIndex);
	
	//Do not use this function on the client side
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly)
		void AddPlayer(APlayerController* PlayerController);
	//Do not use this function on the client side
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly)
		void RemovePlayer(APlayerController* PlayerController);
	
	UFUNCTION(BlueprintCallable,BlueprintPure)
		bool HasItem(URPG_ItemData* Item);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		bool HasItemCount(URPG_ItemData* Item,int Count = 1);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool isInventoryFullOfItems();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetItemCount(URPG_ItemData* Item);
	UFUNCTION(BlueprintCallable)
		void TransferItem(int SlotIndex, URPG_InventoryComponent* Container1 ,int Amount,int TargetSlotIndex, URPG_InventoryComponent* Container2);
	UFUNCTION(BlueprintCallable)
		void TryUseItem(int SlotIndex);
	UFUNCTION(Server,Reliable)
		void TryUseItem_Server(int SlotIndex);
	UFUNCTION(Client,Reliable)
		void TryUseItem_Client(int SlotIndex);
		 UFUNCTION(BlueprintCallable, BlueprintPure)
			 URPG_ItemData*  GetItemDataFromSlot(int SlotIndex); 

	UFUNCTION(BlueprintCallable)
	int GetInventorySize() const;
	UFUNCTION(BlueprintCallable)
	void UpdateAllInventory(APlayerController* PlayerController);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool CanUseItem(int SlotIndex);

	void OnItemAdded(URPG_ItemData* Item_,int Count_);
	void OnItemRemoved(URPG_ItemData* Item_, int Count_);
};
