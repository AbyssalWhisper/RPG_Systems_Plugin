// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "RPG_Systems/InventorySystem/RPG_ItemStruct.h"
#include "RPG_InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventorySlotChange, const FSTR_RPG_ItemSlot&, Item, int32, Slot);

class ARPG_PlayerController;
class ARPG_BaseCharacter;
class UActorComponent;
struct FNetDeltaSerializeInfo;


USTRUCT(BlueprintType)
struct FSTR_RPG_InventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	FSTR_RPG_InventoryList()
		: OwnerComponent(nullptr)
	{
	}

	explicit FSTR_RPG_InventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	void SetOwnerComponent(UActorComponent* InOwnerComponent)
	{
		OwnerComponent = InOwnerComponent;
	}

	void SetNum(int32 NewNum);
	int32 Num() const;
	bool IsValidIndex(int32 Index) const;

	FSTR_RPG_ItemSlot& operator[](int32 Index);
	

	void operator=(TArray<FSTR_RPG_ItemSlot> Items);
	void MarkSlotDirty(int32 SlotIndex);

	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FSTR_RPG_ItemSlot, FSTR_RPG_InventoryList>(Slots, DeltaParms, *this);
	}
public:
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TArray<FSTR_RPG_ItemSlot> Slots;
private:

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FSTR_RPG_InventoryList> : public TStructOpsTypeTraitsBase2<FSTR_RPG_InventoryList>
{
	enum { WithNetDeltaSerializer = true };
};

UCLASS(Blueprintable, BlueprintType,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_SYSTEMS_API URPG_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPG_InventoryComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Slots = 5;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	FSTR_RPG_InventoryList Items;
	UPROPERTY(BlueprintAssignable)
		FOnInventorySlotChange OnInventorySlotChange;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<APlayerController*> Players;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ARPG_BaseCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DecayTick = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DecayFactor = 1;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void BeginDestroy() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	FTimerHandle DecayTimerHandle;
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
	void AddItemAtIndex(FSTR_RPG_ItemSlot Item, int32 SlotIndex, FSTR_RPG_ItemSlot& RemainingItems);
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly)
		void RemoveItemFromIndex(int32 SlotIndex, int32 Count,bool& Sucess);
	void SearchSlotsWithItem(FSTR_RPG_ItemSlot Item, FSTR_RPG_ItemSlot& RemainingItems);
	void SearchEmptySlots(FSTR_RPG_ItemSlot Item, FSTR_RPG_ItemSlot& RemainingItems);

	//void UpdateAllPlayersSlots();
	void UpdatePlayersSlot(int32 SlotIndex);

	
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly)
	void SetItemSlot(FSTR_RPG_ItemSlot Item, int32 SlotIndex);
	void SetItemSlotOnClient(FSTR_RPG_ItemSlot Item, int32 SlotIndex);
	
	//Do not use this function on the client side
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly)
		void AddPlayer(APlayerController* PlayerController);
	//Do not use this function on the client side
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly)
		void RemovePlayer(APlayerController* PlayerController);
	
	UFUNCTION(BlueprintCallable,BlueprintPure)
		bool HasItem(URPG_ItemData* Item);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		bool HasItemCount(URPG_ItemData* Item,int32 Count = 1);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool isInventoryFullOfItems();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetItemCount(URPG_ItemData* Item);
	UFUNCTION(BlueprintCallable)
		void TransferItem(int32 SlotIndex, URPG_InventoryComponent* Container1 ,int32 Amount,int32 TargetSlotIndex, URPG_InventoryComponent* Container2);
	UFUNCTION(BlueprintCallable)
		void TryUseItem(int32 SlotIndex);
	UFUNCTION(Server,Reliable)
		void TryUseItem_Server(int32 SlotIndex);
	UFUNCTION(Client,Reliable)
		void TryUseItem_Client(int32 SlotIndex);
		 UFUNCTION(BlueprintCallable, BlueprintPure)
			 URPG_ItemData*  GetItemDataFromSlot(int32 SlotIndex); 

	UFUNCTION(BlueprintCallable)
	int GetInventorySize() const;
	UFUNCTION(BlueprintCallable)
	void UpdateAllInventory(APlayerController* PlayerController);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool CanUseItem(int32 SlotIndex);

	void OnItemAdded(URPG_ItemData* Item_,int Count_);
	void OnItemRemoved(URPG_ItemData* Item_, int Count_);
	void HandleReplicatedSlotChange(int32 SlotIndex);
};
