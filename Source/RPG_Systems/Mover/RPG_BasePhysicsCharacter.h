// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#define private public
//#include "MoverExamplesCharacter.h"
#undef private
#include "MoverSimulationTypes.h"
#include "GameFramework/Pawn.h"
#include "RPG_BasePhysicsCharacter.generated.h"

/**
 * 
 */

namespace RPG_MovementModes
{
	const FName Climb = TEXT("Climb");
	const FName Crouch = TEXT("Crouch");
}

class UInputAction;
class UCharacterMoverComponent;
struct FInputActionValue;

UCLASS()
class RPG_SYSTEMS_API ARPG_BasePhysicsCharacter : public APawn, public IMoverInputProducerInterface
{
	GENERATED_BODY()
	//void OnProduceInput(float DeltaMs, FMoverInputCmdContext& InputCmdResult) override;

	UPROPERTY()
	bool bWantsCrouch = false;

	UFUNCTION(BlueprintCallable)
	void TryCrouch();
	UFUNCTION(BlueprintCallable)
	void TryUnCrouch();
	UFUNCTION(BlueprintCallable)
	void TryJump();


	UFUNCTION(BlueprintCallable)
	bool IsClimbing() const;
	UFUNCTION(BlueprintCallable)
	bool IsCrouching() const;
	UFUNCTION(BlueprintCallable)
	bool IsWalking() const;

public:
	// Sets default values for this pawn's properties
	ARPG_BasePhysicsCharacter(const FObjectInitializer& ObjectInitializer);


public:
	virtual void PawnClientRestart() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Accessor for the actor's movement component
	UFUNCTION(BlueprintPure, Category = Mover)
	UCharacterMoverComponent* GetMoverComponent() const { return CharacterMotionComponent; }

	// Request the character starts moving with an intended directional magnitude. A length of 1 indicates maximum acceleration.
	UFUNCTION(BlueprintCallable, Category = MoverExamples)
	virtual void RequestMoveByIntent(const FVector& DesiredIntent) { CachedMoveInputIntent = DesiredIntent; }

	// Request the character starts moving with a desired velocity. This will be used in lieu of any other input.
	UFUNCTION(BlueprintCallable, Category = MoverExamples)
	virtual void RequestMoveByVelocity(const FVector& DesiredVelocity) { CachedMoveInputVelocity = DesiredVelocity; }

protected:
	// Entry point for input production. Do not override. To extend in derived character types, override OnProduceInput for native types or implement "Produce Input" blueprint event
	virtual void ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult) override;

	// Override this function in native class to author input for the next simulation frame. Consider also calling Super method.
	virtual void OnProduceInput(float DeltaMs, FMoverInputCmdContext& InputCmdResult);

	// Implement this event in Blueprints to author input for the next simulation frame. Consider also calling Parent event.
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Produce Input", meta = (ScriptName = "OnProduceInput"))
	FMoverInputCmdContext OnProduceInputInBlueprint(float DeltaMs, FMoverInputCmdContext InputCmd);

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

public:
	// Whether or not we author our movement inputs relative to whatever base we're standing on, or leave them in world space
	UPROPERTY(BlueprintReadWrite, Category = MoverExamples)
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

protected:
	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterMoverComponent> CharacterMotionComponent;

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
};
