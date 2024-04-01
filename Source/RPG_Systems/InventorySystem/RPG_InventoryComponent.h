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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSTR_RPG_ItemSlot> Items;
	UPROPERTY(BlueprintAssignable)
		FOnInventorySlotChange OnInventorySlotChange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ARPG_PlayerController*> Players;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ARPG_BaseCharacter* OwnerCharacter;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitContainer();
	UFUNCTION(BlueprintCallable)
	bool TryAddItem(FSTR_RPG_ItemSlot Item, FSTR_RPG_ItemSlot& RemainingItems);
	UFUNCTION(BlueprintCallable)
	void RemoveItem(FName ItemID, int Count);
	UFUNCTION(BlueprintCallable)
		void RemoveItemFromIndex(int index, int Count,bool& Sucess);
	void SearchSlotsWithItem(FSTR_RPG_ItemSlot Item, FSTR_RPG_ItemData* ItemData, FSTR_RPG_ItemSlot& RemainingItems);
	void SearchEmptySlots(FSTR_RPG_ItemSlot Item, FSTR_RPG_ItemData* ItemData, FSTR_RPG_ItemSlot& RemainingItems);
	void AddItemAtIndex(FSTR_RPG_ItemSlot Item,int SlotIndex, FSTR_RPG_ItemData* ItemData, FSTR_RPG_ItemSlot& RemainingItems);
	//void UpdateAllPlayersSlots();
	void UpdatePlayersSlot(int SlotIndex);
	UFUNCTION(BlueprintCallable)
		void SetItemSlot(FSTR_RPG_ItemSlot Item, int SlotIndex);
	UFUNCTION(BlueprintCallable)
		void AddPlayer(ARPG_PlayerController* PlayerController);
	UFUNCTION(BlueprintCallable)
		void RemovePlayer(ARPG_PlayerController* PlayerController);

	 void TransferItem(int SlotIndex, URPG_InventoryComponent* Container1 ,int Amount,int TargetSlotIndex, URPG_InventoryComponent* Container2);
	 UFUNCTION(BlueprintCallable)
		 void TryUseItem(int SlotIndex);
	 
		 UFUNCTION(BlueprintCallable)
			 FSTR_RPG_ItemData GetItemDataFromSlot(int SlotIndex);
		 UFUNCTION(BlueprintCallable)
			 FSTR_RPG_ItemData GetItemData(FSTR_RPG_ItemSlot Item);

};
