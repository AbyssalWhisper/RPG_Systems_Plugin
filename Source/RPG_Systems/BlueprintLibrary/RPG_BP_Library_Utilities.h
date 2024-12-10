// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPG_Systems/InventorySystem/RPG_ItemStruct.h"
#include "Abilities/GameplayAbility.h"
#include "RPG_BP_Library_Utilities.generated.h"

UENUM(BlueprintType)
enum class ERPG_AbilityInputID : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm			UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel			UMETA(DisplayName = "Cancel"),
	// 3 LMB
	Ability1		UMETA(DisplayName = "Ability1"),
	// 4 RMB
	Ability2		UMETA(DisplayName = "Ability2"),
	// 5 Q
	Ability3		UMETA(DisplayName = "Ability3"),
	// 6 E
	Ability4		UMETA(DisplayName = "Ability4"),
	// 7 R
	Ability5		UMETA(DisplayName = "Ability5"),
	// 8 Sprint
	Sprint			UMETA(DisplayName = "Sprint"),
	// 9 Jump
	Jump			UMETA(DisplayName = "Jump"),
	// 10 Run
	Run		UMETA(DisplayName = "Run"),
	// 11 Eat
	Eat		UMETA(DisplayName = "Eat"),
	//12 RadialMenu
	RadialMenu			UMETA(DisplayName = "RadialMenu"),
	//13 NightVision
	NightVision		UMETA(DisplayName = "NightVision")
};

USTRUCT(BlueprintType)
struct  FRPG_GameplayAbilityTargetData_Item : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int SlotIndex;
	FRPG_GameplayAbilityTargetData_Item() {
		SlotIndex = -1;

	}
	FRPG_GameplayAbilityTargetData_Item(int SlotIndex_) {
		SlotIndex = SlotIndex_;

	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FRPG_GameplayAbilityTargetData_Item::StaticStruct();
	}
};

template<>
struct TStructOpsTypeTraits<FRPG_GameplayAbilityTargetData_Item> : public TStructOpsTypeTraitsBase2<FRPG_GameplayAbilityTargetData_Item>
{
	enum
	{
		WithNetSerializer = true	// For now this is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};
/**
 *
 */
/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_BP_Library_Utilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
 
	UFUNCTION(BlueprintCallable)
		static FString NetIDtoString(FUniqueNetIdRepl ID);
	UFUNCTION(BlueprintCallable)
		static FSTR_RPG_ItemData GetItemDataFromSlot(FSTR_RPG_ItemSlot Item);
	UFUNCTION(BlueprintPure)
		static void GetItemFromTargetData(const FGameplayAbilityTargetDataHandle& TargetData, int& SlotIndex_);
	UFUNCTION(BlueprintPure)
		static FGameplayAbilityTargetDataHandle MakeItemTargetData(int SlotIndex_);
	UFUNCTION(BlueprintPure, Category = "System Information")
		static FString GetProjectVersion();
	UFUNCTION(BlueprintCallable, Category = "RPG Utilities")
		static bool InitStatsByCharacterData(UAbilitySystemComponent* AbilitySystemCom, URPG_CharacterDataAsset* CharacterData);
	 
	UFUNCTION(BlueprintCallable)
		static void SetAttributeValue(UAbilitySystemComponent* AbilitySystemComp, FGameplayAttribute Attribute, EGameplayModOp::Type Modifier, float Value);
	UFUNCTION(BlueprintPure, Category = "Snap")
		static FVector SnapPosition(FVector ReferenceLocation,FQuat CamRotation, float SnapSize, FVector InitialPositio, FVector& SnapLocation);
#if WITH_EDITOR
	UFUNCTION(BlueprintCallable)
	static class URPG_ItemData* CreateNewItemAsset(FString Name,FString FolderPath);
#endif

	template<typename T>
	static T* GetComponentByClassWithAutoCast(AActor* Actor)
	{
		if (Actor)
		{
			return Cast<T>(Actor->GetComponentByClass(T::StaticClass()));
		}
		return nullptr;
	}

	
};
