// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#define private public
//#include "MoverExamplesCharacter.h"
#undef private
#include "MoverSimulationTypes.h"
#include "GameFramework/Pawn.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystemInterface.h"


#include "RPG_BaseMoverCharacter.generated.h"

/**
 * 
 */


class UInputAction;
class UCharacterMoverComponent;
class UAbilitySystemComponent;
class URPG_BaseAttributeSet;
class URPG_CharacterDataAsset;
class APlayerController;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerPosses, APlayerController*, PlayerController);


UCLASS()
class RPG_SYSTEMS_API ARPG_BaseMoverCharacter : public APawn, public IAbilitySystemInterface, public IMoverInputProducerInterface
{
	GENERATED_BODY()

public:
	FName CharacterMovementComponentName = (TEXT("CharMoveComp"));
	
	//void OnProduceInput(float DeltaMs, FMoverInputCmdContext& InputCmdResult) override;


public:

	// Sets default values for this pawn's properties
	ARPG_BaseMoverCharacter(const FObjectInitializer& ObjectInitializer);



	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "0 - CharacterData")
	URPG_CharacterDataAsset* CharacterData;

#pragma region GAS
		
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UAbilitySystemComponent* AbilitySystemComp;
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
	// Accessor for the actor's movement component
	UFUNCTION(BlueprintPure, Category = Mover)
	URPG_CharacterMoverComponent* GetMoverComponent() const { return CharacterMotionComponent; }

	// Request the character starts moving with an intended directional magnitude. A length of 1 indicates maximum acceleration.
	UFUNCTION(BlueprintCallable, Category = MoverExamples)
	virtual void RequestMoveByIntent(const FVector& DesiredIntent) { CachedMoveInputIntent = DesiredIntent; }

	// Request the character starts moving with a desired velocity. This will be used in lieu of any other input.
	UFUNCTION(BlueprintCallable, Category = MoverExamples)
	virtual void RequestMoveByVelocity(const FVector& DesiredVelocity) { CachedMoveInputVelocity = DesiredVelocity; }


	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URPG_CharacterMoverComponent> CharacterMotionComponent;


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


	// Whether or not we author our movement inputs relative to whatever base we're standing on, or leave them in world space
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category = MoverExamples)
	bool bUseBaseRelativeMovement = true;

	/**
	 * If true, rotate the Character toward the direction the actor is moving
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MoverExamples)
	bool bOrientRotationToMovement = true;

	/**
	 * If true, the actor will remain vertical despite any rotation applied to the actor
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MoverExamples)
	bool bShouldRemainVertical = true;

	/**
	 * If true, the actor will continue orienting towards the last intended orientation (from input) even after movement intent input has ceased.
	 * This makes the character finish orienting after a quick stick flick from the player.  If false, character will not turn without input.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MoverExamples)
	bool bMaintainLastInputOrientation = false;


#pragma region Input
protected:
	// Entry point for input production. Do not override. To extend in derived character types, override OnProduceInput for native types or implement "Produce Input" blueprint event
	virtual void ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult) override;

	// Override this function in native class to author input for the next simulation frame. Consider also calling Super method.
	virtual void OnProduceInput(float DeltaMs, FMoverInputCmdContext& InputCmdResult);

	// Implement this event in Blueprints to author input for the next simulation frame. Consider also calling Parent event.
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Produce Input", meta = (ScriptName = "OnProduceInput"))
	FMoverInputCmdContext OnProduceInputInBlueprint(float DeltaMs, FMoverInputCmdContext InputCmd);


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* DefaultMappingContext;
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveInputAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookInputAction;

	/** Fly Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* FlyInputAction;
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

	void OnMoveTriggered(const FInputActionValue& Value);
	void OnMoveCompleted(const FInputActionValue& Value);
	void OnLookTriggered(const FInputActionValue& Value);
	void OnLookCompleted(const FInputActionValue& Value);
	void OnFlyTriggered(const FInputActionValue& Value);


	uint8 bHasProduceInputinBpFunc : 1;

#pragma endregion

#pragma region Ragdoll
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ragdoll) 
	bool bCanRagdoll = true;
#pragma endregion

};
