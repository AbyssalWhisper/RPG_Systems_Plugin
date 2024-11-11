// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/BlueprintLibrary/RPG_BP_Library_Utilities.h"

#include "RPG_Systems/Character/RPG_CharacterDataAsset.h"
#include "RPG_Systems/GameplayAbility/RPG_BaseAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPG_Systems/InventorySystem/RPG_ItemData.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "RPG_Systems/GameplayAbility/RPG_HealthAttributeSet.h"
#include "UObject/SavePackage.h"

FSTR_RPG_ItemData URPG_BP_Library_Utilities::GetItemDataFromSlot(FSTR_RPG_ItemSlot Item) {


	return FSTR_RPG_ItemData();
}

FString URPG_BP_Library_Utilities::NetIDtoString(FUniqueNetIdRepl ID) {
	return ID.ToString();
}
void URPG_BP_Library_Utilities::GetItemFromTargetData(const FGameplayAbilityTargetDataHandle& TargetData, int& SlotIndex_) {

	if (!TargetData.Data.IsValidIndex(0))return;

	FRPG_GameplayAbilityTargetData_Item* Data = (FRPG_GameplayAbilityTargetData_Item*)TargetData.Data[0].Get();
	if (Data)
	{
		SlotIndex_ = Data->SlotIndex;

	}
}

FGameplayAbilityTargetDataHandle URPG_BP_Library_Utilities::MakeItemTargetData(int SlotIndex_)
{
	FRPG_GameplayAbilityTargetData_Item* Data = new FRPG_GameplayAbilityTargetData_Item(SlotIndex_);
	FGameplayAbilityTargetDataHandle Handle;
	if (!Data)return Handle;
	Handle.Data.Add(TSharedPtr<FGameplayAbilityTargetData>(Data));
	return Handle;
}

bool FRPG_GameplayAbilityTargetData_Item::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	Ar << SlotIndex;


	bOutSuccess = true;
	return true;
}
FString URPG_BP_Library_Utilities::GetProjectVersion()
{
	FString ProjectVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		ProjectVersion,
		GGameIni
	);
	return ProjectVersion;
}

bool URPG_BP_Library_Utilities::InitStatsByCharacterData(UAbilitySystemComponent* AbilitySystemCom, URPG_CharacterDataAsset* CharacterData)
{
	if (AbilitySystemCom && CharacterData)
	{
		UGameplayEffect* GE_Stats = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Bounty")));
		GE_Stats->DurationPolicy = EGameplayEffectDurationType::Instant;
		GE_Stats->Modifiers.SetNum(30);

		FGameplayModifierInfo ModifierInfo;
		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->MaxHealth);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_HealthAttributeSet::GetMaxHealthAttribute();
		GE_Stats->Modifiers[0] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->MaxHealth);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_HealthAttributeSet::GetHealthAttribute();
		GE_Stats->Modifiers[1] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->HealthRegenRate);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_HealthAttributeSet::GetHealthRegenRateAttribute();
		GE_Stats->Modifiers[2] = ModifierInfo;
 
		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->MaxOxygen);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetMaxOxygenAttribute();
		GE_Stats->Modifiers[3] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->MaxOxygen);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetOxygenAttribute();
		GE_Stats->Modifiers[4] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->OxygenRegenRate);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetOxygenRegenRateAttribute();
		GE_Stats->Modifiers[5] = ModifierInfo;

		 
 

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->MaxThirst);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetMaxThirstAttribute();
		GE_Stats->Modifiers[9] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->MaxThirst);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetThirstAttribute();
		GE_Stats->Modifiers[10] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->ThirstRegenRate);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetThirstRegenRateAttribute();
		GE_Stats->Modifiers[11] = ModifierInfo;

 
		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->MaxStamina);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetMaxStaminaAttribute();
		GE_Stats->Modifiers[12] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->MaxStamina);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetStaminaAttribute();
		GE_Stats->Modifiers[13] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->StaminaRegenRate);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetStaminaRegenRateAttribute();
		GE_Stats->Modifiers[14] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->MaxMana);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetMaxManaAttribute();
		GE_Stats->Modifiers[15] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->MaxMana);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetManaAttribute();
		GE_Stats->Modifiers[16] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->ManaRegenRate);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetManaRegenRateAttribute();
		GE_Stats->Modifiers[17] = ModifierInfo;
 
		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->Attack);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetAttackAttribute();
		GE_Stats->Modifiers[18] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->Armor);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetArmorAttribute();
		GE_Stats->Modifiers[19] = ModifierInfo;
		
		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->MagicArmor);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetMagicArmorAttribute();
		GE_Stats->Modifiers[20] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->DMG_Fire_Multiplier);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetDMG_Fire_MultiplierAttribute();
		GE_Stats->Modifiers[21] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->DMG_Earth_Multiplier);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetDMG_Earth_MultiplierAttribute();
		GE_Stats->Modifiers[22] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->DMG_Wind_Multiplier);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetDMG_Wind_MultiplierAttribute();
		GE_Stats->Modifiers[23] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->DMG_Water_Multiplier);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetDMG_Water_MultiplierAttribute();
		GE_Stats->Modifiers[24] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->DMG_Poison_Multiplier);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetDMG_Poison_MultiplierAttribute();
		GE_Stats->Modifiers[25] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->DMG_Holy_Multiplier);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetDMG_Holy_MultiplierAttribute();
		GE_Stats->Modifiers[26] = ModifierInfo;
		 
		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->DMG_Shadow_Multiplier);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetDMG_Shadow_MultiplierAttribute();
		GE_Stats->Modifiers[27] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->DMG_Ghost_Multiplier);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetDMG_Ghost_MultiplierAttribute();
		GE_Stats->Modifiers[28] = ModifierInfo;

		ModifierInfo.ModifierMagnitude = FScalableFloat(CharacterData->DMG_Undead_Multiplier);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetDMG_Undead_MultiplierAttribute();
		GE_Stats->Modifiers[29] = ModifierInfo;
		
		AbilitySystemCom->ApplyGameplayEffectToSelf(GE_Stats, 1, AbilitySystemCom->MakeEffectContext());
		return true;
	}
	return false;
}
 

void URPG_BP_Library_Utilities::SetAttributeValue(UAbilitySystemComponent* AbilitySystemComp, FGameplayAttribute Attribute ,EGameplayModOp::Type Modifier, float Value)
{
	
	if (AbilitySystemComp)
	{
		UGameplayEffect* GE_ApplyEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("ApplyEffect")));
		GE_ApplyEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
		GE_ApplyEffect->Modifiers.SetNum(1);

		FGameplayModifierInfo ModifierInfo;
		ModifierInfo.ModifierMagnitude = FScalableFloat(Value);
		ModifierInfo.ModifierOp = Modifier;
		ModifierInfo.Attribute = Attribute;
		GE_ApplyEffect->Modifiers[0] = ModifierInfo;

		AbilitySystemComp->ApplyGameplayEffectToSelf(GE_ApplyEffect, 1, AbilitySystemComp->MakeEffectContext());
	}
	
}


FVector URPG_BP_Library_Utilities::SnapPosition(FVector ReferenceLocation,FQuat CamRotation, float SnapSize, FVector InitialPosition, FVector& SnapLocation)
{
	// Assumindo que você está chamando esta função em um membro da classe AMyGridObject

	// Transformar a posição inicial com base na rotação da câmera
	FVector UnrotatedPosition = CamRotation.Inverse() * (ReferenceLocation - InitialPosition);

	// Aplicar o snap à posição não rotacionada
	FVector SnappedPosition = UKismetMathLibrary::Vector_SnappedToGrid(UnrotatedPosition, SnapSize);

	// Não aplicar snap na coordenada z para evitar flicker nas sombras
	SnappedPosition.X = UnrotatedPosition.X;

	// Calcular a diferença entre a posição não rotacionada e a posição snap
	FVector SnapDiff = UnrotatedPosition - SnappedPosition;

	// Rotacionar o vetor snap e adicioná-lo à posição inicial
	FVector RotatedVector = CamRotation.RotateVector(SnappedPosition) + InitialPosition;

	// Definir a posição do objeto com base no vetor rotacionado
	SnapLocation = (RotatedVector);

	// Retornar a diferença de snap
	return SnapDiff;
}
#if WITH_EDITOR
URPG_ItemData* URPG_BP_Library_Utilities::CreateNewItemAsset(FString AssetName, FString FolderPath)
{
	FString PackagePath = FolderPath + AssetName;
	UPackage* Package = CreatePackage(*PackagePath);
	URPG_ItemData* MyDataAsset = NewObject<URPG_ItemData>(
		Package, URPG_ItemData::StaticClass(), *AssetName, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);

	FAssetRegistryModule::AssetCreated(MyDataAsset);
	MyDataAsset->MarkPackageDirty();

	FString FilePath = FPackageName::LongPackageNameToFilename(PackagePath, FPackageName::GetAssetPackageExtension());
	FSavePackageArgs SaveArgs;
	// This is specified just for example
	{
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		SaveArgs.SaveFlags = SAVE_NoError;
	}

	UPackage::Save(Package, MyDataAsset, *FilePath, SaveArgs);

	return MyDataAsset;
}

#endif
