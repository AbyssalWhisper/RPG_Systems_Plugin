// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoverExamplesCharacter.h"
#include "MoverSimulationTypes.h"
#include "GameFramework/Pawn.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystemInterface.h"
#include "RPG_Systems/RPG_AbilitySystemComponent.h"


#include "RPG_BaseMoverCharacter.generated.h"

/**
 * 
 */


class URPG_GameplayAbilitySet;
class UCapsuleComponent;
class UInputAction;
class UCharacterMoverComponent;
class UAbilitySystemComponent;
class URPG_BaseAttributeSet;
class URPG_CharacterDataAsset;
class APlayerController;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerPosses, APlayerController*, PlayerController);


UCLASS()
class RPG_SYSTEMS_API ARPG_BaseMoverCharacter : public AMoverExamplesCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public: 
	
	//void OnProduceInput(float DeltaMs, FMoverInputCmdContext& InputCmdResult) override;


public:

	// Sets default values for this pawn's properties
	ARPG_BaseMoverCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "0 - CharacterData")
	URPG_GameplayAbilitySet* PlayerAbilitiesDataAsset;
	void OnAbilityInputPressed(int32 InputID);
	void OnAbilityInputReleased(int32 InputID);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "0 - CharacterData")
	URPG_CharacterDataAsset* CharacterData;

#pragma region GAS
		
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	URPG_AbilitySystemComponent* AbilitySystemComp;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URPG_BaseAttributeSet* AttributesSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<class URPG_GameplayAbility>> CharacterAbilities;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> Effects;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

#pragma endregion 
	
#pragma region GameplayTags

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void AddGameplayTag(FGameplayTag TagToAdd);
	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void RemoveGameplayTag(FGameplayTag TagToRemove);

	// Tags
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayTags")
	FGameplayTag DeadTag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayTags")
	FGameplayTag FullHealthTag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayTags")
	FGameplayTag FullManaTag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayTags")
	FGameplayTag FullStaminaTag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayTags")
	FGameplayTag FullFoodTag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayTags")
	FGameplayTag FullOxygenTag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayTags")
	FGameplayTag FullThirstTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayTags")
	FGameplayTag AttackTag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayTags")
	FGameplayTag StunTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayTags")
	FGameplayTag SprintingTag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayTags")
	FGameplayTag RunningTag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayTags")
	FGameplayTag GodModeTag;
#pragma endregion GameplayTags

#pragma region States
	//States 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	bool bIsDead = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	bool bIsDrinking = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	bool bIsEating = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bEmptyFood = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bEmptyThirst = false;
#pragma endregion States

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TargetCapsuleHalfHeight = 40;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TargetCapsuleRadius = 40;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CrouchTargetCapsuleHalfHeight = 40;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CrouchTargetCapsuleRadius = 40;

	UFUNCTION(BlueprintCallable)
	void TryCrouch();
	UFUNCTION(BlueprintCallable)
	void TryUnCrouch();
	UFUNCTION(BlueprintCallable)
	void TryJump();

	UPROPERTY()
	bool bWantsCrouch = false;
	bool bIsCrouching = false;
	UFUNCTION(BlueprintCallable)
	bool IsCrouching() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UCapsuleComponent* CapsuleComp;

	virtual void PawnClientRestart() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma region Mover


#pragma endregion

public:
#pragma region Posses

	void PossessedBy(AController* NewController) override;

	virtual void UnPossessed() override;
	UFUNCTION(Client, Reliable)
	void Client_PossessedByPlayer(APlayerController* PlayerController);
	UFUNCTION(Client, Reliable)
	void Client_UnPossessedByPlayer(APlayerController* OldPlayerController);
	UFUNCTION(BlueprintNativeEvent)
	void PossessedByPlayerOnClient(APlayerController* PlayerController);
	UFUNCTION(BlueprintNativeEvent)
	void UnPossessedByPlayerOnClient(APlayerController* OldPlayerController);
	UPROPERTY(BlueprintAssignable)
	FOnPlayerPosses OnPossesedByPlayer;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerPosses OnUnPossessedByPlayer;

#pragma endregion 


	/**
	 * If true, the actor will remain vertical despite any rotation applied to the actor
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MoverExamples)
	bool bShouldRemainVertical = true;

	/**
	 * If true, the actor will continue orienting towards the last intended orientation (from input) even after movement intent input has ceased.
	 * This makes the character finish orienting after a quick stick flick from the player.  If false, character will not turn without input.
	 */



#pragma region Input

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	FVector ProcessMoveInput(const FVector& MoveInput);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* DefaultMappingContext;
private:

	
	FVector LastAffirmativeMoveInput = FVector::ZeroVector;	// Movement input (intent or velocity) the last time we had one that wasn't zero

	FVector CachedMoveInputIntent = FVector::ZeroVector;
	FVector CachedMoveInputVelocity = FVector::ZeroVector;

	FRotator CachedTurnInput = FRotator::ZeroRotator;
	FRotator CachedLookInput = FRotator::ZeroRotator;

	bool bIsJumpJustPressed = false;
	bool bIsJumpPressed = false;
	bool bIsFlyingActive = false;
	bool bShouldToggleFlying = false;




	uint8 bHasProduceInputinBpFunc : 1;

#pragma endregion

#pragma region Ragdoll
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ragdoll) 
	bool bCanRagdoll = true;
#pragma endregion

};
