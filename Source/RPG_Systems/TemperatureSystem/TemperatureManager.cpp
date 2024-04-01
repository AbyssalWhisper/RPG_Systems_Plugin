


#include "RPG_Systems/TemperatureSystem/TemperatureManager.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
// Sets default values for this component's properties
UTemperatureManager::UTemperatureManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UTemperatureManager::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority()) {
 
			FTimerHandle Handle;
			FTimerManager& TimerManager = GetWorld()->GetTimerManager();
			TimerManager.SetTimer(Handle, this, &UTemperatureManager::TickTemperature, 10, true);
 
	}
	// ...
	
}


 
void UTemperatureManager::OnRep_FinalTemperature()
{
	OnTemperatureChange(FinalTemperature);
}

void UTemperatureManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UTemperatureManager, FinalTemperature, COND_None, REPNOTIFY_OnChanged);
}

// Called every frame
void UTemperatureManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTemperatureManager::TickTemperature()
{
	if (TemperatureCurve)
	{
		FinalTemperature = Temperature + TemperatureCurve->GetFloatValue(GetTimerOfDay());
		OnRep_FinalTemperature();
		if (DamageCurve)
		{
			float Value = FMath::Abs(DamageCurve->GetFloatValue(FinalTemperature) * 5);
			ExecuteTickTemperature( Value );
		}
	}
	 
}
 

float UTemperatureManager::GetTimerOfDay_Implementation()
{
	return 0.0f;
}

void UTemperatureManager::ExecuteTickTemperature_Implementation(float Damage)
{
}

void UTemperatureManager::OnTemperatureChange_Implementation(float temperature)
{
}

