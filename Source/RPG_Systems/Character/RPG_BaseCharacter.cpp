// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Character/RPG_BaseCharacter.h"
#include "RPG_Systems/GameplayAbility/RPG_BaseAttributeSet.h"
#include "RPG_Systems/Character/RPG_CharacterMovementComponent.h"
#include "RPG_Systems/GameplayAbility/RPG_PlayerAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "RPG_Systems/GameplayAbility/RPG_GameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "RPG_Systems/Character/RPG_CharacterDataAsset.h"
#include "GameplayEffect.h"
#include "RPG_Systems/GameplayEffects/GE_FoodDamage.h"
#include "RPG_Systems/RPG_AbilitySystemComponent.h"
#include "RPG_Systems/GameplayAbility/RPG_HealthAttributeSet.h"
// Sets default values
ARPG_BaseCharacter::ARPG_BaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<URPG_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComp = CreateDefaultSubobject<URPG_AbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComp->SetIsReplicated(true);
	AttributesSet = CreateDefaultSubobject<URPG_BaseAttributeSet>("PlayerAttributes");

}

// Called when the game starts or when spawned
void ARPG_BaseCharacter::BeginPlay()
{
	Super::BeginPlay(); 
	//StartCancelAttackWindow();
	if (!AbilitySystemComp)return;

		AbilitySystemComp->InitAbilityActorInfo(this, this);

	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComp)return;

	for (TSubclassOf<class UGameplayEffect>& StartupEffects : Effects)
	{
		FGameplayEffectSpecHandle Handle;
		if (StartupEffects)
		{
			//Handle = AbilitySystemComp->MakeOutgoingSpec(StartupEffects, 0, AbilitySystemComp->MakeEffectContext());
			//AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());
			FGameplayEffectContextHandle GameplayHandle;
			//AbilitySystemComp->ApplyGameplayEffectToSelf(StartupEffects::)
			AbilitySystemComp->BP_ApplyGameplayEffectToSelf(StartupEffects, 1, GameplayHandle);
		}
	}


	for (TSubclassOf<URPG_GameplayAbility>& StartupAbility : CharacterAbilities)
	{
		if (StartupAbility)
		{
			AbilitySystemComp->GiveAbility(
				FGameplayAbilitySpec(StartupAbility, 1));
		}
	}

	if (!CharacterData)return;

	//InitStats();

	for (TSubclassOf<class UGameplayEffect>& StartupEffects : CharacterData->Effects)
	{
		FGameplayEffectSpecHandle Handle;
		if (StartupEffects)
		{
			//Handle = AbilitySystemComp->MakeOutgoingSpec(StartupEffects, 0, AbilitySystemComp->MakeEffectContext());
			//AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());
			FGameplayEffectContextHandle GameplayHandle;
			//AbilitySystemComp->ApplyGameplayEffectToSelf(StartupEffects::)
			AbilitySystemComp->BP_ApplyGameplayEffectToSelf(StartupEffects, 1, GameplayHandle);
		}
	}

	for (TSubclassOf<URPG_GameplayAbility>& StartupAbility : CharacterData->CharacterAbilities)
	{
		if (StartupAbility)
		{
			AbilitySystemComp->GiveAbility(
				FGameplayAbilitySpec(StartupAbility, 1));
		}
	}

}

// Called every frame
void ARPG_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ShouldTickCancelMoveCheck) {
		CancelMoveTick();
	}
}



void ARPG_BaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARPG_BaseCharacter, MovementStates);
	DOREPLIFETIME(ARPG_BaseCharacter, bIsDead);
	DOREPLIFETIME(ARPG_BaseCharacter, bIsEating);
	DOREPLIFETIME(ARPG_BaseCharacter, bIsDrinking);
}

UAbilitySystemComponent* ARPG_BaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

float ARPG_BaseCharacter::GetMoveSpeed()
{
	if (!CharacterData)return 100;

	switch (MovementStates)
	{
	case SlowWalk:
		return (CharacterData->SlowWalkMoveSpeed + AttributesSet->GetExtraMoveSpeed()) * AttributesSet->GetMoveSpeedMultiplier();
		break;
	case Walk:
		return (CharacterData->WalkMoveSpeed + AttributesSet->GetExtraMoveSpeed()) * AttributesSet->GetMoveSpeedMultiplier();
		break;
	case Run:
		return (CharacterData->RunMoveSpeed + AttributesSet->GetExtraMoveSpeed()) * AttributesSet->GetMoveSpeedMultiplier();
		break;
	case Sprint:
		return (CharacterData->SprintMoveSpeed + AttributesSet->GetExtraMoveSpeed()) * AttributesSet->GetMoveSpeedMultiplier();
		break;
	default:
		return (CharacterData->WalkMoveSpeed + AttributesSet->GetExtraMoveSpeed()) * AttributesSet->GetMoveSpeedMultiplier();
		break;
	}
}

void ARPG_BaseCharacter::InitStats()
{
	
	URPG_BP_Library_Utilities::InitStatsByCharacterData(AbilitySystemComp, CharacterData);
	{
	//InitRegenEffects
	UGameplayEffect* GE_RegenRates = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("GE_FoodRegen")));
	GE_RegenRates->DurationPolicy = EGameplayEffectDurationType::Infinite;
	GE_RegenRates->Modifiers.SetNum(3);
	GE_RegenRates->Period = FScalableFloat(1);

	FGameplayModifierInfo ModifierInfo;
	FAttributeBasedFloat AttributeBased;

	AttributeBased.BackingAttribute = FGameplayEffectAttributeCaptureDefinition(URPG_BaseAttributeSet::GetFoodRegenRateAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
	AttributeBased.AttributeCalculationType = EAttributeBasedFloatCalculationType::AttributeMagnitude;
 
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(AttributeBased);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = URPG_BaseAttributeSet::GetFoodAttribute();
	GE_RegenRates->Modifiers[0] = ModifierInfo;

	AttributeBased.BackingAttribute = FGameplayEffectAttributeCaptureDefinition(URPG_BaseAttributeSet::GetThirstRegenRateAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
	AttributeBased.AttributeCalculationType = EAttributeBasedFloatCalculationType::AttributeMagnitude;
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(AttributeBased);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = URPG_BaseAttributeSet::GetThirstAttribute();
	GE_RegenRates->Modifiers[1] = ModifierInfo;

	AttributeBased.BackingAttribute = FGameplayEffectAttributeCaptureDefinition(URPG_HealthAttributeSet::GetHealthRegenRateAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
	AttributeBased.AttributeCalculationType = EAttributeBasedFloatCalculationType::AttributeMagnitude;
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(AttributeBased);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = URPG_HealthAttributeSet::GetHealthAttribute();
	GE_RegenRates->Modifiers[2] = ModifierInfo;

	if (DeadTag.IsValid())
	{
		GE_RegenRates->OngoingTagRequirements.IgnoreTags.AddTag(DeadTag);
	}

	AbilitySystemComp->ApplyGameplayEffectToSelf(GE_RegenRates, 1, AbilitySystemComp->MakeEffectContext());
	}

	{
		UGameplayEffect* GE_StaminaRegenRate = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("GE_StaminaRegenRate")));
		FGameplayModifierInfo ModifierInfo;
		FAttributeBasedFloat AttributeBased;

		GE_StaminaRegenRate->DurationPolicy = EGameplayEffectDurationType::Infinite;
		GE_StaminaRegenRate->Modifiers.SetNum(1);
		GE_StaminaRegenRate->Period = FScalableFloat(CharacterData->StaminaRegenTickInterval);

		if (RunningTag.IsValid())
		{
			GE_StaminaRegenRate->OngoingTagRequirements.IgnoreTags.AddTag(RunningTag);
		}
		if (SprintingTag.IsValid())
		{
			GE_StaminaRegenRate->OngoingTagRequirements.IgnoreTags.AddTag(SprintingTag);
		}
		if (DeadTag.IsValid())
		{
			GE_StaminaRegenRate->OngoingTagRequirements.IgnoreTags.AddTag(DeadTag);
		}

		AttributeBased.BackingAttribute = FGameplayEffectAttributeCaptureDefinition(URPG_BaseAttributeSet::GetStaminaRegenRateAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
		AttributeBased.AttributeCalculationType = EAttributeBasedFloatCalculationType::AttributeMagnitude;
		ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(AttributeBased);
		ModifierInfo.ModifierOp = EGameplayModOp::Additive;
		ModifierInfo.Attribute = URPG_BaseAttributeSet::GetStaminaAttribute();

		GE_StaminaRegenRate->Modifiers[0] = ModifierInfo;
		AbilitySystemComp->ApplyGameplayEffectToSelf(GE_StaminaRegenRate, 1, AbilitySystemComp->MakeEffectContext());
	}
}

void ARPG_BaseCharacter::SetMovementState(const E_WalkMovementStates SetMovementState)
{
	MovementStates = SetMovementState;
}

void ARPG_BaseCharacter::GiveAbility(TSubclassOf<URPG_GameplayAbility> Ability)
{
	AbilitySystemComp->GiveAbility(
		FGameplayAbilitySpec(Ability, 1));
}

void ARPG_BaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if (!AbilitySystemComp)return;
	float FallVelocity = GetVelocity().Z;
	if (FallVelocity < CharacterData->FallVelocityInitDamage)
	{
		UGameplayEffect* GE_Landed = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Bounty")));
		GE_Landed->DurationPolicy = EGameplayEffectDurationType::Instant;
		GE_Landed->Modifiers.SetNum(1);

		if (GodModeTag.IsValid())
		{
			GE_Landed->ApplicationTagRequirements.IgnoreTags.AddTag(GodModeTag);
		}

		FGameplayModifierInfo ModifierInfo;
		ModifierInfo.ModifierMagnitude = FScalableFloat(((FallVelocity / 100) * CharacterData->FallDamage_Multiplier));
		ModifierInfo.ModifierOp = EGameplayModOp::Additive;
		ModifierInfo.Attribute = URPG_HealthAttributeSet::GetHealthAttribute();
		GE_Landed->Modifiers[0] = ModifierInfo;
		
		AbilitySystemComp->ApplyGameplayEffectToSelf(GE_Landed, 1, AbilitySystemComp->MakeEffectContext());
	}
}

void ARPG_BaseCharacter::AddGameplayTag(FGameplayTag TagToAdd)
{
	GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
	GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, 1);
}

void ARPG_BaseCharacter::RemoveGameplayTag(FGameplayTag TagToRemove)
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagToRemove);
}

void ARPG_BaseCharacter::BindASCInput(UInputComponent* InputComp)
{
	if (!ASCInputBound && AbilitySystemComp && InputComp)
	{
		AbilitySystemComp->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),FString("CancelTarget"), FString("ERPG_AbilityInputID"), static_cast<int32>(ERPG_AbilityInputID::Confirm), static_cast<int32>(ERPG_AbilityInputID::Cancel)));
		ASCInputBound = true;
	}
}

void ARPG_BaseCharacter::CancelMoveTick()
{

		if (ShouldMoveCancel() && !QueueAttackAbility)
		{
			if (GetNetMode() != ENetMode::NM_Client) {
				StopMontage();
			}
			else
			{
				StopMontage();
				Server_StopMontage();
			}
		}

}

bool ARPG_BaseCharacter::ShouldMoveCancel()
{
	if (IsLocallyControlled())
	{
		return GetLastMovementInputVector().Size() > 0;
	}
	return false;
}

void ARPG_BaseCharacter::StopMontage()
{
	StopAnimMontage(GetMesh()->GetAnimInstance()->GetCurrentActiveMontage());
}

bool ARPG_BaseCharacter::Server_StopMontage_Validate()
{
	return true;
}

void ARPG_BaseCharacter::Server_StopMontage_Implementation()
{
	StopMontage();
}

void ARPG_BaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AbilitySystemComp)
	{
		AbilitySystemComp->RefreshAbilityActorInfo();
	}
	if (auto a = Cast<APlayerController>(NewController))
	{
		Client_PossessedBy(a);
	}
}

void ARPG_BaseCharacter::UnPossessed()
{
	Super::UnPossessed();
	if (auto a = Cast<APlayerController>(GetController()))
	{
		Client_UnPossessed(a);
	}
}

void ARPG_BaseCharacter::Client_PossessedBy_Implementation(APlayerController* PlayerController)
{
	PossessedOnClient(PlayerController);
}

void ARPG_BaseCharacter::Client_UnPossessed_Implementation(APlayerController* OldPlayerController)
{
	UnPossessedOnClient(OldPlayerController);
}

void ARPG_BaseCharacter::PossessedOnClient_Implementation(APlayerController* PlayerController)
{
}

void ARPG_BaseCharacter::UnPossessedOnClient_Implementation(APlayerController* OldPlayerController)
{
}

void ARPG_BaseCharacter::TryCancelAttack()
{
	/// <summary>
	/// 
	/// 
	/// Rever 
	/// 
	/// 
	/// </summary>
	if (QueueAttackAbility)
	{
		StopMontage();
		if (AbilitySystemComp->TryActivateAbilityByClass(QueueAttackAbility, true))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "SucessActiveAttack");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "FailActiveAttack");
		}
		QueueAttackAbility = nullptr;
		IsCancelAttackWindow = false;
	}
}

void ARPG_BaseCharacter::TryAttack()
{
	if (GetVelocity().Size() >= 100 && !InCombo && (MovementStates == E_WalkMovementStates::Sprint) )
	{
		AbilitySystemComp->TryActivateAbilityByClass(DefaultRunningAttack);
	}
	else
	{
		if (!InCombo)
		{
			if (!AbilitySystemComp->HasMatchingGameplayTag(AttackTag))
			{
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "TryAttack");

				AbilitySystemComp->TryActivateAbilityByClass(DefaultAttackAbility);
			}
		}
		else
		{
			if (CurrentNextAttackAbility)
			{
				QueueAttackAbility = CurrentNextAttackAbility;
				if (IsCancelAttackWindow)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "TryActiveNextAbilityAttack");

					TryCancelAttack();
					return;
				}
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "NotCancelWindow");

			}
		}
	}
}

void ARPG_BaseCharacter::StopCancelAttackWindow_Implementation()
{
	IsCancelAttackWindow = false;
}

void ARPG_BaseCharacter::StartCancelAttackWindow_Implementation()
{
	IsCancelAttackWindow = true;
	TryCancelAttack();
}

void ARPG_BaseCharacter::ClearCurrentNextAbilityAttack_Implementation()
{
	CurrentNextAttackAbility = nullptr;
	InCombo = false;
}

void ARPG_BaseCharacter::SetCurrentNextComboAbility_Implementation(TSubclassOf<UGameplayAbility> Ability)
{
	CurrentNextAttackAbility = Ability;
	InCombo = true;
}

void ARPG_BaseCharacter::StartMoveCancelTick_Implementation()
{
	ShouldTickCancelMoveCheck = true;
}

void ARPG_BaseCharacter::StopMoveCancelTick_Implementation()
{
	ShouldTickCancelMoveCheck = false;
}

 

void ARPG_BaseCharacter::OnFoodChanged_Implementation(float Value)
{
	if (Value > 0 && bEmptyFood)
	{
		AbilitySystemComp->RemoveActiveGameplayEffect(FoodDamageHandle);
		bEmptyFood = false;
	}
}

void ARPG_BaseCharacter::OnThirstChanged_Implementation(float Value)
{
	if (Value > 0 && bEmptyThirst)
	{
		AbilitySystemComp->RemoveActiveGameplayEffect(ThirstDamageHandle);
		bEmptyThirst = false;
	}
}

void ARPG_BaseCharacter::OnHealthChanged_Implementation(float Value)
{
}

void ARPG_BaseCharacter::OnStaminaChanged_Implementation(float Value)
{
}

void ARPG_BaseCharacter::OnOxygenChanged_Implementation(float Value)
{
}

void ARPG_BaseCharacter::OnManaChanged_Implementation(float Value)
{
}

void ARPG_BaseCharacter::OnFoodEmpty_Implementation()
{
	if (!bEmptyFood)
	{
		UGameplayEffect* GE_FoodDamage = NewObject<UGE_FoodDamage>(GetTransientPackage(), FName(TEXT("GE_FoodDamage")));
		GE_FoodDamage->DurationPolicy = EGameplayEffectDurationType::Infinite;
		GE_FoodDamage->Modifiers.SetNum(1);
		GE_FoodDamage->Period = FScalableFloat(CharacterData->FoodDamageTickInterval);

		FGameplayModifierInfo ModifierInfo;
		ModifierInfo.ModifierMagnitude = FScalableFloat(-CharacterData->FoodDamage);
		ModifierInfo.ModifierOp = EGameplayModOp::Additive;
 
		if (GodModeTag.IsValid())
		{
			GE_FoodDamage->OngoingTagRequirements.IgnoreTags.AddTag(GodModeTag);
		}

		ModifierInfo.Attribute = URPG_HealthAttributeSet::GetHealthAttribute();
		GE_FoodDamage->Modifiers[0] = ModifierInfo;
		FoodDamageHandle = AbilitySystemComp->ApplyGameplayEffectToSelf(GE_FoodDamage, 1, AbilitySystemComp->MakeEffectContext());
		bEmptyFood = true;
	}
}

void ARPG_BaseCharacter::OnThirstEmpty_Implementation()
{
	if (!bEmptyThirst)
	{
		UGameplayEffect* GE_ThirstDamage = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("GE_ThirstDamage")));
		GE_ThirstDamage->DurationPolicy = EGameplayEffectDurationType::Infinite;
		GE_ThirstDamage->Modifiers.SetNum(1);
		GE_ThirstDamage->Period = FScalableFloat(CharacterData->ThirstDamageTickInterval);

		FGameplayModifierInfo ModifierInfo;
		ModifierInfo.ModifierMagnitude = FScalableFloat(-CharacterData->ThirstDamage);
		ModifierInfo.ModifierOp = EGameplayModOp::Additive;
		 
		if (GodModeTag.IsValid())
		{
			GE_ThirstDamage->OngoingTagRequirements.IgnoreTags.AddTag(GodModeTag);
		}
		ModifierInfo.Attribute = URPG_HealthAttributeSet::GetHealthAttribute();
		GE_ThirstDamage->Modifiers[0] = ModifierInfo;
		FoodDamageHandle = AbilitySystemComp->ApplyGameplayEffectToSelf(GE_ThirstDamage, 1, AbilitySystemComp->MakeEffectContext());
		bEmptyThirst = true;
	}
}



void ARPG_BaseCharacter::OnHealthEmpty_Implementation()
{
	if (!bIsDead)
	{
		bIsDead = true;
		OnDie();
	}
}

void ARPG_BaseCharacter::OnStaminaEmpty_Implementation()
{
}

void ARPG_BaseCharacter::OnOxygenEmpty_Implementation()
{
}

void ARPG_BaseCharacter::OnManaEmpty_Implementation()
{
}

void ARPG_BaseCharacter::OnDie()
{
	BP_OnDie();
	AddGameplayTag(DeadTag);
}

void ARPG_BaseCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	if (Controller)
	{
		if (auto PlayerController = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

#pragma region Input


// Called to bind functionality to input
void ARPG_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	 
	BindASCInput(PlayerInputComponent);
}

#pragma endregion