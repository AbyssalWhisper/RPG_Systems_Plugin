// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "RPG_Systems/Character/RPG_CharacterLibrary.h"
#include "RPG_Systems/Character/Interfaces/RPG_CancelWindowsInterfaces.h"
#include "RPG_Systems/BlueprintLibrary/RPG_BP_Library_Utilities.h"
#include "RPG_Systems/General_Interfaces/RPG_Attributes_Interface.h"
#include "RPG_BaseCharacter.generated.h"

class URPG_CharacterDataAsset;
class URPG_BaseAttributeSet;
class URPG_GameplayAbility;
UCLASS()
class RPG_SYSTEMS_API ARPG_BaseCharacter : public ACharacter , public IAbilitySystemInterface , public IRPG_CancelWindowsInterfaces , public IRPG_Attributes_Interface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPG_BaseCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UAbilitySystemComponent* AbilitySystemComp;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		URPG_BaseAttributeSet* AttributesSet;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "0 - CharacterData")
	URPG_CharacterDataAsset* CharacterData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TArray<TSubclassOf<class URPG_GameplayAbility>> CharacterAbilities;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TArray<TSubclassOf<class UGameplayEffect>> Effects;
#pragma region GameplayTags

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
		void AddGameplayTag(FGameplayTag TagToAdd);
	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
		void RemoveGameplayTag(FGameplayTag TagToRemove);

	// Tags
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category = "GameplayTags")
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere ,Replicated)
		bool bIsDead = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
		bool bIsDrinking = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
		bool bIsEating = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bEmptyFood = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bEmptyThirst = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
		TEnumAsByte<E_WalkMovementStates> MovementStates = E_WalkMovementStates::Run;
#pragma endregion States

#pragma region GameplayEffectsHandle

	FActiveGameplayEffectHandle FoodDamageHandle;
	FActiveGameplayEffectHandle ThirstDamageHandle;
#pragma endregion GameplayEffectsHandle

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ShouldTickCancelMoveCheck = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldServerCancelAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCancelAttackWindow = false;

#pragma region Combo
	// Combo
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UGameplayAbility> CurrentNextAttackAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UGameplayAbility> QueueAttackAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UGameplayAbility> DefaultAttackAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UGameplayAbility> DefaultRunningAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool InCombo;

#pragma endregion Combo

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable)
	float GetMoveSpeed();

	UFUNCTION()
		void InitStats();
	UFUNCTION(BlueprintCallable)
		void SetMovementState(const E_WalkMovementStates SetMovementState);

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
		void GiveAbility(TSubclassOf<URPG_GameplayAbility> Ability);

	virtual void Landed(const FHitResult& Hit) override;

	//Bind AbilityInput
	UFUNCTION(BlueprintCallable)
		void BindASCInput(UInputComponent* InputComp);
	bool ASCInputBound = false;
	// Cancel Move Tick
	
	UFUNCTION(BlueprintCallable)
	void CancelMoveTick();
	UFUNCTION(BlueprintCallable)
	bool ShouldMoveCancel();
	UFUNCTION(BlueprintCallable)
		void StopMontage();
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void Server_StopMontage();
	bool Server_StopMontage_Validate();
	void Server_StopMontage_Implementation();
 

	void TryCancelAttack();
	UFUNCTION(BlueprintCallable)
	void TryAttack();

	//Combo Interaces
#pragma region ComboActionInterface

	// Cancel Interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopCancelAttackWindow();
	virtual void StopCancelAttackWindow_Implementation() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartCancelAttackWindow();
	virtual void StartCancelAttackWindow_Implementation() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ClearCurrentNextAbilityAttack();
	virtual void ClearCurrentNextAbilityAttack_Implementation() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCurrentNextComboAbility(TSubclassOf<UGameplayAbility> Ability);
	virtual void SetCurrentNextComboAbility_Implementation(TSubclassOf<UGameplayAbility> Ability) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartMoveCancelTick();
	virtual void StartMoveCancelTick_Implementation() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopMoveCancelTick();
	virtual void StopMoveCancelTick_Implementation() override;
#pragma endregion ComboActionInterface

#pragma region GameplayAbilites


	UFUNCTION(BlueprintNativeEvent)
		void OnFoodChanged(float Value);
	virtual void OnFoodChanged_Implementation(float Value);
	UFUNCTION(BlueprintNativeEvent)
		void OnThirstChanged(float Value);
	virtual void OnThirstChanged_Implementation(float Value);
	UFUNCTION(BlueprintNativeEvent)
		void OnHealthChanged(float Value);
	virtual void OnHealthChanged_Implementation(float Value);
	UFUNCTION(BlueprintNativeEvent)
		void OnStaminaChanged(float Value);
	virtual void OnStaminaChanged_Implementation(float Value);
	UFUNCTION(BlueprintNativeEvent)
		void OnOxygenChanged(float Value);
	virtual void OnOxygenChanged_Implementation(float Value);
	UFUNCTION(BlueprintNativeEvent)
		void OnManaChanged(float Value);
	virtual void OnManaChanged_Implementation(float Value);
	UFUNCTION(BlueprintNativeEvent)
		void OnFoodEmpty();
	virtual void OnFoodEmpty_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void OnThirstEmpty();
	virtual void OnThirstEmpty_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void OnHealthEmpty();
	virtual void OnHealthEmpty_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void OnStaminaEmpty();
	virtual void OnStaminaEmpty_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void OnOxygenEmpty();
	virtual void OnOxygenEmpty_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void OnManaEmpty();
	virtual void OnManaEmpty_Implementation();

	UFUNCTION()
		void OnDie(/*AActor* Causer, bool IsPlayer, const FString& PlayerName*/);
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDie"))
		void BP_OnDie(/*AActor* Causer, bool IsPlayer, const FString& PlayerName*/);

#pragma endregion GameplayAbilites

	void PossessedBy(AController* NewController) override;

	virtual void UnPossessed() override;
	UFUNCTION(Client,Reliable)
	void Client_PossessedBy(APlayerController* PlayerController);
	UFUNCTION(Client,Reliable)
	void Client_UnPossessed(APlayerController* OldPlayerController);
	UFUNCTION(BlueprintNativeEvent)
	void PossessedOnClient(APlayerController* PlayerController);
	UFUNCTION(BlueprintNativeEvent)
	void UnPossessedOnClient(APlayerController* OldPlayerController);
};
