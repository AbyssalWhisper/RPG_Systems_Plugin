#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ChaosCharacterMovementModeBP_Library.generated.h"

class UChaosCharacterMovementMode;

UCLASS()
class RPG_SYSTEMS_API UChaosCharacterMovementModeBP_Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RPG|Mover|ChaosCharacterMovementMode", meta = (WorldContext = "MovementMode", DefaultToSelf = "MovementMode"))
	static bool ShouldEnableConstraint(const UChaosCharacterMovementMode* MovementMode);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RPG|Mover|ChaosCharacterMovementMode", meta = (WorldContext = "MovementMode", DefaultToSelf = "MovementMode"))
	static float GetTargetHeight(const UChaosCharacterMovementMode* MovementMode);

	UFUNCTION(BlueprintCallable, Category = "RPG|Mover|ChaosCharacterMovementMode", meta = (WorldContext = "MovementMode", DefaultToSelf = "MovementMode"))
	static void SetTargetHeight(UChaosCharacterMovementMode* MovementMode, float Value);

	UFUNCTION(BlueprintCallable, Category = "RPG|Mover|ChaosCharacterMovementMode", meta = (WorldContext = "MovementMode", DefaultToSelf = "MovementMode"))
	static void ClearTargetHeight(UChaosCharacterMovementMode* MovementMode);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RPG|Mover|ChaosCharacterMovementMode", meta = (WorldContext = "MovementMode", DefaultToSelf = "MovementMode"))
	static float GetGroundQueryRadius(const UChaosCharacterMovementMode* MovementMode);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RPG|Mover|ChaosCharacterMovementMode", meta = (WorldContext = "MovementMode", DefaultToSelf = "MovementMode"))
	static float GetMaxWalkSlopeCosine(const UChaosCharacterMovementMode* MovementMode);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RPG|Mover|ChaosCharacterMovementMode", meta = (WorldContext = "MovementMode", DefaultToSelf = "MovementMode"))
	static bool ShouldCharacterRemainUpright(const UChaosCharacterMovementMode* MovementMode);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RPG|Mover|ChaosCharacterMovementMode", meta = (WorldContext = "MovementMode", DefaultToSelf = "MovementMode"))
	static float GetMaxSpeed(const UChaosCharacterMovementMode* MovementMode);

	UFUNCTION(BlueprintCallable, Category = "RPG|Mover|ChaosCharacterMovementMode", meta = (WorldContext = "MovementMode", DefaultToSelf = "MovementMode"))
	static void OverrideMaxSpeed(UChaosCharacterMovementMode* MovementMode, float Value);

	UFUNCTION(BlueprintCallable, Category = "RPG|Mover|ChaosCharacterMovementMode", meta = (WorldContext = "MovementMode", DefaultToSelf = "MovementMode"))
	static void ClearMaxSpeedOverride(UChaosCharacterMovementMode* MovementMode);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RPG|Mover|ChaosCharacterMovementMode", meta = (WorldContext = "MovementMode", DefaultToSelf = "MovementMode"))
	static float GetAcceleration(const UChaosCharacterMovementMode* MovementMode);

	UFUNCTION(BlueprintCallable, Category = "RPG|Mover|ChaosCharacterMovementMode", meta = (WorldContext = "MovementMode", DefaultToSelf = "MovementMode"))
	static void OverrideAcceleration(UChaosCharacterMovementMode* MovementMode, float Value);

	UFUNCTION(BlueprintCallable, Category = "RPG|Mover|ChaosCharacterMovementMode", meta = (WorldContext = "MovementMode", DefaultToSelf = "MovementMode"))
	static void ClearAccelerationOverride(UChaosCharacterMovementMode* MovementMode);
};
