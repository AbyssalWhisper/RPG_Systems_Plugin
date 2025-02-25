// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_BaseMoverCharacter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MoverFunctionLibrary.generated.h"

namespace RPG_MovementModes
{
	const FName Climbing = TEXT("Climbing");
	const FName Crouch = TEXT("Crouch");
}

USTRUCT(BlueprintType)
struct RPG_SYSTEMS_API FRPG_MoverInputTags : public FMoverDataStructBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = Mover)
	FGameplayTagContainer Tags = FGameplayTagContainer();


	// @return newly allocated copy of this FMoverExampleAbilityInputs. Must be overridden by child classes
	virtual FMoverDataStructBase* Clone() const override
	{
		// TODO: ensure that this memory allocation jives with deletion method
		FRPG_MoverInputTags* CopyPtr = new FRPG_MoverInputTags(*this);
		return CopyPtr;
	}

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override
	{
		Super::NetSerialize(Ar, Map, bOutSuccess);

		Tags.NetSerialize(Ar, Map, bOutSuccess);

		bOutSuccess = true;
		return true;
	}

	virtual UScriptStruct* GetScriptStruct() const override { return StaticStruct(); }

	virtual void ToString(FAnsiStringBuilderBase& Out) const override
	{
		Super::ToString(Out);
	}
	bool operator==(const FRPG_MoverInputTags& Other) const
	{
		return Tags == Other.Tags;
			
	}
	bool operator!=(const FRPG_MoverInputTags& Other) const { return !operator==(Other); }
	
	
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override { Super::AddReferencedObjects(Collector); }
	virtual bool ShouldReconcile(const FMoverDataStructBase& AuthorityState) const override;
	virtual void Interpolate(const FMoverDataStructBase& From, const FMoverDataStructBase& To, float Pct) override;
	virtual void Merge(const FMoverDataStructBase& From) override;
};

template<>
struct TStructOpsTypeTraits<FRPG_MoverInputTags> : public TStructOpsTypeTraitsBase2< FRPG_MoverInputTags >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

USTRUCT(BlueprintType)
struct RPG_SYSTEMS_API FRPG_RawInput : public FMoverDataStructBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = Mover)
	FVector MoveInput = FVector();


	// @return newly allocated copy of this FMoverExampleAbilityInputs. Must be overridden by child classes
	virtual FMoverDataStructBase* Clone() const override
	{
		// TODO: ensure that this memory allocation jives with deletion method
		FRPG_RawInput* CopyPtr = new FRPG_RawInput(*this);
		return CopyPtr;
	}

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override
	{
		Super::NetSerialize(Ar, Map, bOutSuccess);

		MoveInput.NetSerialize(Ar, Map, bOutSuccess);

		bOutSuccess = true;
		return true;
	}

	virtual UScriptStruct* GetScriptStruct() const override { return StaticStruct(); }

	virtual void ToString(FAnsiStringBuilderBase& Out) const override
	{
		Super::ToString(Out);
	}
	bool operator==(const FRPG_RawInput& Other) const
	{
		return MoveInput == Other.MoveInput;
			
	}
	bool operator!=(const FRPG_RawInput& Other) const { return !operator==(Other); }
	
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override { Super::AddReferencedObjects(Collector); }
	virtual bool ShouldReconcile(const FMoverDataStructBase& AuthorityState) const override;
	virtual void Interpolate(const FMoverDataStructBase& From, const FMoverDataStructBase& To, float Pct) override;
	virtual void Merge(const FMoverDataStructBase& From) override;
};

template<>
struct TStructOpsTypeTraits<FRPG_RawInput> : public TStructOpsTypeTraitsBase2< FRPG_RawInput >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API UMoverFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	


	UFUNCTION(BlueprintCallable)
	static void SetTransforms_WorldSpace(UPARAM(ref) FMoverTickEndData& OutputState, FVector WorldLocation, FRotator WorldOrient, FVector WorldVelocity, UPrimitiveComponent* Base=nullptr, FName BaseBone = NAME_None);
	UFUNCTION(BlueprintCallable)
	static void ChaosDrawDebugCapsuleRay(FVector Start, FVector End, float HalfHeight = 80.0f, float Radius = 50.0f, FRotator Rotation = FRotator::ZeroRotator, FLinearColor DrawColor = FLinearColor::Red, bool bPersistentLines = false, float DrawTime = 1.0f, uint8 DepthPriority = 0, float Thickness = 1.0f);
	UFUNCTION(BlueprintCallable)
	static void ChaosDrawDebugSphereRay(FVector Start, FVector End, float Radius = 50, int32 Segments = 12, FLinearColor DrawColor = FLinearColor::Red, bool bPersistentLines = false, float DrawTime = 1, uint8 DepthPriority = 0, float Thickness = 1.f);
	UFUNCTION(BlueprintCallable)
	static void ChaosDrawDebugBoxRay(FVector Start, FVector End, FVector const& Extent, FRotator const& Rotation, FLinearColor DrawColor = FLinearColor::Red, bool bPersistentLines = false, float DrawTime = 1.0f, uint8 DepthPriority = 0, float Thickness = 1.0f);

	UFUNCTION(BlueprintCallable)
	static void ChaosDrawDebugLine(FVector Start, FVector End, float DrawTime = 1, FLinearColor DrawColor = FLinearColor::Red, bool bPersistentLines = false, uint8 DepthPriority = 0, float Thickness = 1.f) ;
	UFUNCTION(BlueprintCallable)
	static void ChaosDrawDebugSphere(FVector const& Center, float Radius = 50, int32 Segments = 12, FLinearColor DrawColor = FLinearColor::Red, bool bPersistentLines = false, float DrawTime = 1, uint8 DepthPriority = 0, float Thickness = 1.f);
	UFUNCTION(BlueprintCallable)
	static void ChaosDrawDebugCapsule(FVector const& Center, float HalfHeight = 80.0f, float Radius = 50.0f, FRotator Rotation = FRotator::ZeroRotator, FLinearColor DrawColor = FLinearColor::Red, bool bPersistentLines = false, float DrawTime = 1.0f, uint8 DepthPriority = 0, float Thickness = 1.0f);
	UFUNCTION(BlueprintCallable)
	static void ChaosDrawDebugBox(FVector const& Center, FVector const& Extent, FRotator const& Rotation, FLinearColor DrawColor = FLinearColor::Red, bool bPersistentLines = false, float DrawTime = 1.0f, uint8 DepthPriority = 0, float Thickness = 1.0f);
	UFUNCTION(BlueprintCallable)
	static void ChaosDrawDebugPoint(FVector const& Position, FLinearColor DrawColor = FLinearColor::Red, bool bPersistentLines = false, float DrawTime = 1.0f, uint8 DepthPriority = 0, float Thickness = 1.0f);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	static bool IsMovementMode(UMoverComponent* Target,FName CompareName);
	UFUNCTION(BlueprintCallable)
	static void AddInputTags(UPARAM(Ref) FMoverDataCollection& Collection,FGameplayTagContainer InputTags);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static bool HasInputTag(UPARAM(Ref) FMoverDataCollection& Collection, FGameplayTag InputTag);
	UFUNCTION(BlueprintCallable)
	static void AddRawInput(UPARAM(Ref) FMoverDataCollection& Collection,FRPG_RawInput RawInput);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static FRPG_RawInput GetRawInput(UPARAM(Ref) const FMoverDataCollection& Collection);

};
