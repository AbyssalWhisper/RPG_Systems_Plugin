

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TemperatureManager.generated.h"

class UCurveFloat;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class RPG_SYSTEMS_API UTemperatureManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTemperatureManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* TemperatureCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* DamageCurve;
public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_FinalTemperature)
		float FinalTemperature = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Temperature = 0;
	UFUNCTION(BlueprintCallable)
		virtual void OnRep_FinalTemperature();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY()
	class UAbilitySystemComponent* AbilitySystemComp;
	void TickTemperature();
	UFUNCTION( BlueprintNativeEvent , BlueprintCallable)
		float GetTimerOfDay();
		virtual float GetTimerOfDay_Implementation();
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
			void ExecuteTickTemperature(float Damage);
		virtual void ExecuteTickTemperature_Implementation(float Damage);
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
			void OnTemperatureChange(float Value);
		virtual void OnTemperatureChange_Implementation(float Value);
};
