// Copyright Zurichi

#pragma once

#include "CoreMinimal.h"
#include "DefaultToInstancedObject.h"
#include "ReplicatedObject.generated.h"


UENUM(BlueprintType)
enum EReplicationState
{
	//This UObject is considered for replication.
	Replicates,
	//This UObject is not considered for replication.
	DoNotReplicate,
};

/**
 * 
 */
UCLASS(Abstract)
class BETTERUTILITIES_API UReplicatedObject : public UDefaultToInstancedObject
{
	GENERATED_BODY()

public:

	//UObject interface implementation
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, struct FOutParmRec* OutParms, FFrame* Stack) override;
	//End of implementation


public:

	//Is this UObject replicated?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replicated UObject")
	TEnumAsByte<EReplicationState> ReplicationState = EReplicationState::Replicates;

	
};
