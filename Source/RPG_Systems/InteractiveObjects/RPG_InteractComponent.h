// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPG_Systems/InventorySystem/RPG_ItemStruct.h"
#include "RPG_InteractComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractTargetActorChange, AActor*, InteractActor);
class URPG_InventoryComponent;
class ARPG_PlayerController;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_SYSTEMS_API URPG_InteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPG_InteractComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		URPG_InventoryComponent* CurrentContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* InteractTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bInteractTrace = true;

	UPROPERTY(BlueprintAssignable)
		FOnInteractTargetActorChange OnInteractTargetActorChange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ARPG_PlayerController* PlayerController;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void Interact();
	void SetInteractTarget(AActor* InteractActor);
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void Server_Interact(AActor* TargetActor);
	bool Server_Interact_Validate(AActor* TargetActor);
	void Server_Interact_Implementation(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_UpdateInventorySlot(URPG_InventoryComponent* Inventory, FSTR_RPG_ItemSlot Item,int SlotIndex);
	void Client_UpdateInventorySlot_Implementation(URPG_InventoryComponent* Inventory, FSTR_RPG_ItemSlot Item, int SlotIndex);
	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_UpdateAllSlotsContainer(URPG_InventoryComponent* Inventory,const TArray<FSTR_RPG_ItemSlot>& Items);
		void Client_UpdateAllSlotsContainer_Implementation(URPG_InventoryComponent* Inventory, const TArray<FSTR_RPG_ItemSlot>& Items);

	//Try PickupItemServer
		UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void TryPickupItemOnServer(ARPG_MasterItem* ItemActor);
		bool TryPickupItemOnServer_Validate(ARPG_MasterItem* ItemActor);
		void TryPickupItemOnServer_Implementation(ARPG_MasterItem* ItemActor);

		//Try PickupItemServer
		UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
			void Server_TransferItem(int SlotIndex, URPG_InventoryComponent* Container1, int Amount, int TargetSlotIndex, URPG_InventoryComponent* Container2);
		bool Server_TransferItem_Validate(int SlotIndex, URPG_InventoryComponent* Container1, int Amount, int TargetSlotIndex, URPG_InventoryComponent* Container2);
		void Server_TransferItem_Implementation(int SlotIndex, URPG_InventoryComponent* Container1, int Amount, int TargetSlotIndex, URPG_InventoryComponent* Container2);

		//Set Container
		void SetContainer(URPG_InventoryComponent* Container);
};
