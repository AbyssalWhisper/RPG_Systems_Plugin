


#include "RPG_Systems/Chat/RPG_ChatComponent.h"

#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
URPG_ChatComponent::URPG_ChatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void URPG_ChatComponent::BeginPlay()
{
	Super::BeginPlay();


	// ...
	
}


// Called every frame
void URPG_ChatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool URPG_ChatComponent::PartyCheck_Implementation(APlayerState* Target)
{
	return true;
}

void URPG_ChatComponent::SendMessage(const FString& Message,const EChatChannel Channel)
{
	APlayerController* Player = Cast<APlayerController>(GetOwner());
	switch (Channel) {
	case EChatChannel::GlobalChat:
		if (Player)
		{
			if (Player->PlayerState)
			{
				FString PlayerName = Player->PlayerState->GetPlayerName();
				Server_SendMessage(Message,Channel, Player->PlayerState);
				return;
			}
		}
		else
		{
			Server_SendMessage(Message,Channel,nullptr);
		}
		break;
	case EChatChannel::PartyChat:
		
		if (Player)
		{
			if (Player->PlayerState)
			{
				FString PlayerName = Player->PlayerState->GetPlayerName();
				Server_SendMessage(Message,Channel, Player->PlayerState);
				return;
			}
		}
		break;
	case EChatChannel::Clan:
		break;
	case EChatChannel::System:
		Client_SendMessage("[System] " + Message,Channel,nullptr);
		break;
	default: ;
	}
	
	 
	
}

void URPG_ChatComponent::Client_SendMessage_Implementation(const FString& Message, const EChatChannel Channel,  APlayerState* MassageOwner)
{
	OnChatReceivedMessage.Broadcast(Message,Channel, MassageOwner);
}

void URPG_ChatComponent::MultiCast_SendMessage_Implementation(const FString& Message,const EChatChannel Channel,  APlayerState* MassageOwner)
{
 
	OnChatReceivedMessage.Broadcast(Message,Channel, MassageOwner);
}

void URPG_ChatComponent::Server_SendMessage_Implementation(const FString& Message,const EChatChannel Channel, APlayerState* MassageOwner)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerController::StaticClass(), FoundActors);
	APlayerController* PlayerOwner = Cast<APlayerController>(GetOwner());
	switch (Channel) {
	case EChatChannel::GlobalChat:
		
		for (auto FoundActor : FoundActors)
		{
			if (APlayerController* Player = Cast<APlayerController>(FoundActor))
			{
				UActorComponent* Component = Player->GetComponentByClass(URPG_ChatComponent::StaticClass());
				URPG_ChatComponent* Chat = Cast<URPG_ChatComponent>(Component);
				if (Component &&  Chat)
				{
					Chat->Client_SendMessage(Message,Channel, MassageOwner);
				}
			}
		}
		break;
	case EChatChannel::PartyChat:
		for (auto FoundActor : FoundActors)
		{
			
			if (APlayerController* Player = Cast<APlayerController>(FoundActor))
			{
				if (PartyCheck(Player->PlayerState))
				{
					UActorComponent* Component = Player->GetComponentByClass(URPG_ChatComponent::StaticClass());
					URPG_ChatComponent* Chat = Cast<URPG_ChatComponent>(Component);
					if (Component &&  Chat)
					{
						Chat->Client_SendMessage(Message,Channel, MassageOwner);
					}
				}
				
			}
		}
		break;
	case EChatChannel::Clan:
		break;
	case EChatChannel::System:
		break;
	default: ;
	}
	
 
}

 
