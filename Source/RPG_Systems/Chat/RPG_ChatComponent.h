                                                        

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPG_ChatComponent.generated.h"

UENUM(BlueprintType)
enum class EChatChannel : uint8
{
	GlobalChat,
	PartyChat,
	Clan,//	Não implementado 
	System,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRPG_OnChatReceivedMessage,FString,Message,EChatChannel, Channel,APlayerState*,Player);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable )
class RPG_SYSTEMS_API URPG_ChatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPG_ChatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable)
	FRPG_OnChatReceivedMessage OnChatReceivedMessage;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintNativeEvent)
	bool PartyCheck(APlayerState* Target);
	UFUNCTION(BlueprintCallable)
	void SendMessage(const FString& Message,const EChatChannel Channel);
	UFUNCTION(Server,Reliable)
	void Server_SendMessage(const FString& Message,const EChatChannel Channel,APlayerState* MassageOwner);
	UFUNCTION(Client,Reliable)
	void Client_SendMessage(const FString& Message,const EChatChannel Channel, APlayerState* MassageOwner);
	UFUNCTION(NetMulticast,Reliable)
	void MultiCast_SendMessage(const FString& Message,const EChatChannel Channel, APlayerState* MassageOwner);
};
