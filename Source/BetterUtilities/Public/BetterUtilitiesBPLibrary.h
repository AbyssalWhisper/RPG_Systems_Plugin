// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "UObject/Object.h"
#include "UObject/UnrealType.h"
#include "Kismet/GameplayStatics.h"
#include "BetterUtilitiesBPLibrary.generated.h"

class UReplicatedObject;


DECLARE_LOG_CATEGORY_EXTERN(EasyLog, Log, All);

UENUM(BlueprintType)
enum EEasylog : uint8
{
    /** Not used */
    NoLogging		= 0,

    /** Always prints a fatal error to console (and log file) and crashes (even if logging is disabled) */
    Fatal,

    /** 
     * Prints an error to console (and log file). 
     * Commandlets and the editor collect and report errors. Error messages result in commandlet failure.
     */
    Error,

    /** 
     * Prints a warning to console (and log file).
     * Commandlets and the editor collect and report warnings. Warnings can be treated as an error.
     */
    Warning,

    /** 
     * Prints a verbose message to a log file (if Verbose logging is enabled for the given category, 
     * usually used for detailed logging) 
     */
    Verbose,

};
	
USTRUCT(BlueprintType)
struct  FMinMaxValues {
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    double Min;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    double Max;
    FMinMaxValues() :
        Min(0.0),
        Max(0.0)
    {
    }
    FMinMaxValues(double min, double max) :
    Min(min),
    Max(max)
    {
    }
};

UENUM(BlueprintType)
enum class EPlatform : uint8
{
    Unknown,
    Windows,
    Mac,
    Linux,
    Android,
    iOS,
    PS4,
    XboxOne,
    Switch,
    Other
};

UENUM(BlueprintType)
enum class ETrueFalse : uint8
{
    True_,
    False_
};
/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

UENUM(BlueprintType)
enum class EPropertyAccess : uint8
{
    None UMETA(DisplayName = "None"),
    BlueprintReadOnly UMETA(DisplayName = "BlueprintReadOnly"),
    BlueprintReadWrite UMETA(DisplayName = "BlueprintReadWrite")
};



UCLASS()
class BETTERUTILITIES_API UBetterUtilities : public UBlueprintFunctionLibrary
{
    GENERATED_UCLASS_BODY()
public:
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "BetterUtilities sample test testing"), Category = "BetterUtilitiesTesting")
    static float BetterUtilitiesSampleFunction(float Param);
    UFUNCTION(BlueprintCallable)
    static void ClearWidget(UPARAM(ref) UUserWidget*& Widget);
    UFUNCTION(BlueprintCallable,meta = (ExpandEnumAsExecs = "Platform"))
    static void GetPlatform(EPlatform& Platform);
    UFUNCTION(BlueprintCallable,BlueprintPure)
    static EPlatform GetPlatformEnum();

    UFUNCTION(BlueprintCallable,meta = (ExpandEnumAsExecs = "TrueFalse"))
    static void RunInEditor(ETrueFalse& TrueFalse);
    UFUNCTION(BlueprintCallable,BlueprintPure)
    static double InverseLerp(float start, float end, float value)
    { 
        return (value - start) / (end - start);
    } 

    UFUNCTION(BlueprintCallable,BlueprintPure)
    static float GetPercent(const float value,const float maxValue)
    {
        return (value / maxValue) * 100.0f;
    }
    
    UFUNCTION(BlueprintCallable,BlueprintPure)
    static float GetPercentFromBytes(int64 CurrentBytes, int64 TotalBytes)
    {
        if (TotalBytes == 0)
        {
            return 0.0f; // Evita divisão por zero
        }

        return static_cast<float>(CurrentBytes) / static_cast<float>(TotalBytes) * 100.0f;
    }

    UFUNCTION(BlueprintCallable,BlueprintPure)
    static float GetAlphaPercentFromBytesT(int64 CurrentBytes, int64 TotalBytes)
    {
        if (TotalBytes == 0)
        {
            return 0.0f; // Evita divisão por zero
        }

        return static_cast<float>(CurrentBytes) / static_cast<float>(TotalBytes);
    }
    
    UFUNCTION(BlueprintCallable,BlueprintPure)
    static float GetAlphaPercent(const float value,const float maxValue)
    {
        return (value / maxValue);
    }

    UFUNCTION(BlueprintCallable, BlueprintPure)
    static float GetPercentFromAttribute(const UAbilitySystemComponent* AbilitySystemComponent,const FGameplayAttribute Attribute,const FGameplayAttribute MaxAttribute)
    {
        const float MaxValue = AbilitySystemComponent->GetNumericAttribute(MaxAttribute);
        const float CurrentValue = AbilitySystemComponent->GetNumericAttribute(Attribute);
        return GetPercent(CurrentValue, MaxValue);
    }

    UFUNCTION(BlueprintCallable, BlueprintPure)
    static float GetAlphaPercentFromAttribute(const UAbilitySystemComponent* AbilitySystemComponent,const FGameplayAttribute Attribute,const FGameplayAttribute MaxAttribute)
    {
        const float MaxValue = AbilitySystemComponent->GetNumericAttribute(MaxAttribute);
        const float CurrentValue = AbilitySystemComponent->GetNumericAttribute(Attribute);
        return GetAlphaPercent(CurrentValue, MaxValue);
    }


    UFUNCTION(BlueprintCallable,BlueprintPure)
    static FVector GetDownVector(USceneComponent* Target)
    {
        if (Target) return Target->GetUpVector() * -1;
        return FVector::ZeroVector;
    }
    UFUNCTION(BlueprintCallable,BlueprintPure)
    static FVector GetLeftVector(USceneComponent* Target)
    {
        if (Target) return Target->GetRightVector() * -1;
        return FVector::ZeroVector;
    }

    UFUNCTION(BlueprintCallable,BlueprintPure)
    static FVector GetBackward(USceneComponent* Target)
    {
        if (Target) return Target->GetForwardVector() * -1;
        return FVector::ZeroVector;
    }

    UFUNCTION(BlueprintCallable,meta = (ExpandEnumAsExecs = "TrueFalse"))
    static void ClassIsChildOfBranch(TSubclassOf<class UObject> TestClass, TSubclassOf<class UObject> ParentClass,ETrueFalse& TrueFalse)
    {
        if (((*ParentClass != NULL) && (*TestClass != NULL)) ? (*TestClass)->IsChildOf(*ParentClass) : false)
        {
            TrueFalse = ETrueFalse::True_;
        }
        else
        {
            TrueFalse = ETrueFalse::False_;
        } 
    }
#pragma region Cast
    UFUNCTION(BlueprintCallable, Category="Actor",  meta=(WorldContext="WorldContextObject", DeterminesOutputType="GameInstanceClass",ExpandEnumAsExecs = "TrueFalse") )
    static UObject* GetGameInstanceWithAutoCast (const UObject* WorldContextObject, TSubclassOf<UGameInstance> GameInstanceClass,ETrueFalse& TrueFalse)
    {
        if (GameInstanceClass && WorldContextObject && WorldContextObject->GetWorld()) {
            UGameInstance*Game =  Cast<UGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject->GetWorld()));
            if (Game && Game->GetClass()->IsChildOf(GameInstanceClass)) {
                TrueFalse = ETrueFalse::True_;
                return Game;
            } 
        }
        TrueFalse = ETrueFalse::False_;
        return nullptr;
    }

    UFUNCTION(BlueprintPure, Category = "Actor",meta=(WorldContext="WorldContextObject"))
    static UObject* GetGameInstanceWithAutoCastPure(const UObject* WorldContextObject, TSubclassOf<UGameInstance> GameInstanceClass) {
        if (GameInstanceClass && WorldContextObject && WorldContextObject->GetWorld()) {
            UGameInstance* Game = Cast<UGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject->GetWorld()));
            if (Game && Game->GetClass()->IsChildOf(GameInstanceClass)) {
                return Game;
            }
        }
        return nullptr;
    }

    UFUNCTION(BlueprintCallable, Category = "Game", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "GameModeClass",ExpandEnumAsExecs = "TrueFalse"))
    static AGameModeBase* GetGameModeWithAutoCast(const UObject* WorldContextObject, TSubclassOf<AGameModeBase> GameModeClass,ETrueFalse& TrueFalse) {
        if (GameModeClass && WorldContextObject && WorldContextObject->GetWorld()) {
            AGameModeBase* GameMode = UGameplayStatics::GetGameMode(WorldContextObject->GetWorld());
            if (GameMode && GameMode->GetClass()->IsChildOf(GameModeClass)) {
                TrueFalse = ETrueFalse::True_;
                return GameMode;
            }
        }
        TrueFalse = ETrueFalse::False_;
        return nullptr;
    }

    UFUNCTION(BlueprintPure, Category = "Game", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "GameModeClass" ))
    static AGameModeBase* GetGameModeWithAutoCastPure(const UObject* WorldContextObject, TSubclassOf<AGameModeBase> GameModeClass ) {
        if (GameModeClass && WorldContextObject && WorldContextObject->GetWorld()) {
            AGameModeBase* GameMode = UGameplayStatics::GetGameMode(WorldContextObject->GetWorld());
            if (GameMode && GameMode->GetClass()->IsChildOf(GameModeClass)) { 
                return GameMode;
            }
        } 
        return nullptr;
    }
    UFUNCTION(BlueprintCallable, Category = "Game", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "GameStateClass",ExpandEnumAsExecs = "TrueFalse"))
    static AGameStateBase* GetGameStateWithAutoCast(const UObject* WorldContextObject, TSubclassOf<AGameStateBase> GameStateClass,ETrueFalse& TrueFalse) {
        if (GameStateClass && WorldContextObject && WorldContextObject->GetWorld()) {
            AGameStateBase* GameState = WorldContextObject->GetWorld()->GetGameState();
            if (GameState && GameState->GetClass()->IsChildOf(GameStateClass)) {
                TrueFalse = ETrueFalse::True_;
                return GameState;
            }
        }
        TrueFalse = ETrueFalse::False_;
        return nullptr;
    }

    UFUNCTION(BlueprintPure, Category = "Game", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "GameStateClass"))
    static AGameStateBase* GetGameStateWithAutoCastPure(const UObject* WorldContextObject, TSubclassOf<AGameStateBase> GameStateClass) {
        if (GameStateClass && WorldContextObject && WorldContextObject->GetWorld()) {
            AGameStateBase* GameState = WorldContextObject->GetWorld()->GetGameState();
            if (GameState && GameState->GetClass()->IsChildOf(GameStateClass)) {
                return GameState;
            }
        }
        return nullptr;
    }

    UFUNCTION(BlueprintCallable, Category = "Player", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "PlayerStateClass",ExpandEnumAsExecs = "TrueFalse"))
    static APlayerState* GetPlayerStateWithAutoCast(const UObject* WorldContextObject,TSubclassOf<APlayerState> PlayerStateClass, int32 PlayerIndex,ETrueFalse& TrueFalse) {
        if (PlayerStateClass && WorldContextObject && WorldContextObject->GetWorld()) {
            const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject->GetWorld(), PlayerIndex);
            if (PlayerController) {
                APlayerState* PlayerState = PlayerController->PlayerState;
                if (PlayerState && PlayerState->GetClass()->IsChildOf(PlayerStateClass)) {
                    TrueFalse = ETrueFalse::True_;
                    return PlayerState;
                }
            }
        }
        TrueFalse = ETrueFalse::False_;
        return nullptr;
    }

    UFUNCTION(BlueprintPure, Category = "Player", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "PlayerStateClass"))
    static APlayerState* GetPlayerStateWithAutoCastPure(const UObject* WorldContextObject, TSubclassOf<APlayerState> PlayerStateClass, int32 PlayerIndex) {
        if (PlayerStateClass && WorldContextObject && WorldContextObject->GetWorld()) {
            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject->GetWorld(), PlayerIndex);
            if (PlayerController) {
                APlayerState* PlayerState = PlayerController->PlayerState;
                if (PlayerState && PlayerState->GetClass()->IsChildOf(PlayerStateClass)) {
                    return PlayerState;
                }
            }
        }
        return nullptr;
    }

    UFUNCTION(BlueprintCallable, Category = "Player", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "PlayerControllerClass",ExpandEnumAsExecs = "TrueFalse"))
    static APlayerController* GetPlayerControllerWithAutoCast(const UObject* WorldContextObject, TSubclassOf<APlayerController> PlayerControllerClass, int32 PlayerIndex,ETrueFalse& TrueFalse) {
        if (PlayerControllerClass && WorldContextObject && WorldContextObject->GetWorld()) {
            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject->GetWorld(), PlayerIndex);
            if (PlayerController && PlayerController->GetClass()->IsChildOf(PlayerControllerClass)) {
                TrueFalse = ETrueFalse::True_;
                return PlayerController;
            }
        }
        TrueFalse = ETrueFalse::False_;
        return nullptr;
    }

    UFUNCTION(BlueprintPure, Category = "Player", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "PlayerControllerClass"))
    static APlayerController* GetPlayerControllerWithAutoCastPure(const UObject* WorldContextObject, TSubclassOf<APlayerController> PlayerControllerClass, int32 PlayerIndex) {
        if (PlayerControllerClass && WorldContextObject && WorldContextObject->GetWorld()) {
            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject->GetWorld(), PlayerIndex);
            if (PlayerController && PlayerController->GetClass()->IsChildOf(PlayerControllerClass)) {
                return PlayerController;
            }
        }
        return nullptr;
    }

    UFUNCTION(BlueprintCallable, Category = "Delay")
    static FTimerHandle EditorDelay( FTimerDynamicDelegate InputDelegate, float DelayTime);
    UFUNCTION(BlueprintCallable, Category = "Delay")
    static void ClearTimerInEditor(FTimerHandle TimerHandle);
#pragma endregion Cast


#pragma region Mesh
    //Mesh

    UFUNCTION(BlueprintCallable, Category = "Mesh")
    static void SetMaterials(UMeshComponent* TargetMesh, TArray<UMaterialInterface*> Materials);
    UFUNCTION(BlueprintCallable, Category = "Mesh")
    static void SetSkeletalMeshAndMaterials(USkeletalMeshComponent* TargetMesh,USkeletalMesh* Mesh, TArray<UMaterialInterface*> Materials);
    UFUNCTION(BlueprintCallable, Category = "Mesh")
    static void SetStaticMeshAndMaterials(UStaticMeshComponent* TargetMesh,UStaticMesh* Mesh, TArray<UMaterialInterface*> Materials);

    UFUNCTION(BlueprintCallable, Category = "Mesh", meta=(AutoCreateRefTerm="Materials"))
    static UStaticMeshComponent* CreateStaticMeshAndSetMaterials(USceneComponent* AttachToComponent, UStaticMesh* Mesh,const  TArray<UMaterialInterface*>& Materials) {
        UStaticMeshComponent* NewStaticMeshComponent = NewObject<UStaticMeshComponent>(AttachToComponent);
        NewStaticMeshComponent->RegisterComponent();
        if (NewStaticMeshComponent) {
            NewStaticMeshComponent->SetStaticMesh(Mesh);
            SetMaterials(NewStaticMeshComponent, Materials);
            NewStaticMeshComponent->AttachToComponent(AttachToComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
            return NewStaticMeshComponent;
        }
        return nullptr;
    }

    UFUNCTION(BlueprintCallable, Category = "Mesh", meta=(AutoCreateRefTerm="Materials"))
    static USkeletalMeshComponent* CreateSkeletalMeshAndSetMaterials(USceneComponent* AttachToComponent, USkeletalMesh* Mesh,const TArray<UMaterialInterface*>& Materials) {
        USkeletalMeshComponent* NewSkeletalMeshComponent = NewObject<USkeletalMeshComponent>(AttachToComponent);
        NewSkeletalMeshComponent->RegisterComponent();
        if (NewSkeletalMeshComponent) {
            NewSkeletalMeshComponent->SetSkeletalMesh(Mesh);
            SetMaterials(NewSkeletalMeshComponent, Materials);
            NewSkeletalMeshComponent->AttachToComponent(AttachToComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
            return NewSkeletalMeshComponent;
        }
        return nullptr;
    }
#pragma endregion Mesh

    UFUNCTION(BlueprintCallable)
    static void AddWidgetToClientViewport(UWidget* Widget,int Z_Order);
    UFUNCTION(BlueprintPure)
    static UObject* GetWorldContextObjectFromGEngine();
    UFUNCTION(BlueprintPure, meta = (DisplayName = "GameplayTagToString",CompactNodeTitle="->",BlueprintAutocast),Category = "Converter")
    static FString Conv_GameplayTagToString(FGameplayTag Tag);
    UFUNCTION(BlueprintPure, meta = (DisplayName = "AutoLoadAnimMontage",BlueprintAutocast),Category = "Converter")
    static UAnimMontage* AutoLoadAnimMontage(TSoftObjectPtr<UAnimMontage> SoftAnimMontage);
    UFUNCTION(BlueprintPure, meta = (DisplayName = "AutoLoadGameplayAbility",BlueprintAutocast),Category = "Converter")
    static TSubclassOf<UGameplayAbility> AutoLoadGameplayAbility(TSoftClassPtr<UGameplayAbility> SoftGameplayAbility);
    UFUNCTION(BlueprintPure, meta = (DisplayName = "AutoLoadSkeletalMesh",BlueprintAutocast),Category = "Converter")
    static USkeletalMesh* AutoLoadSkeletalMesh(TSoftObjectPtr<USkeletalMesh> SoftSkeletalMesh);
    UFUNCTION(BlueprintPure, meta = (DisplayName = "AutoLoadStaticMesh",BlueprintAutocast),Category = "Converter")
    static UStaticMesh* AutoLoadStaticMesh(TSoftObjectPtr<UStaticMesh> SoftStaticMesh);
    UFUNCTION(BlueprintPure, meta = (DisplayName = "AutoLoadMaterial",BlueprintAutocast),Category = "Converter")
    static UMaterialInterface* AutoLoadMaterial(TSoftObjectPtr<UMaterialInterface> Material);
    UFUNCTION(BlueprintPure, meta = (DisplayName = "AutoLoadMaterials",BlueprintAutocast),Category = "Converter")
    static TArray<UMaterialInterface*> AutoLoadMaterials(TArray<TSoftObjectPtr<UMaterialInterface>> Materials);
    UFUNCTION(BlueprintCallable, Category = "PlayerController")
    static void ShowMouseCursor(bool bShow);
    UFUNCTION(BlueprintCallable, Category="Collision", meta=(bIgnoreSelf="true",Distance="100", WorldContext="WorldContextObject", AutoCreateRefTerm="ActorsToIgnore", AdvancedDisplay="TraceColor,TraceHitColor,DrawTime", Keywords="raycast"))
    static bool SimpleLineTraceSingleByChannel(const UObject* WorldContextObject,FVector StartLocation,FRotator Direction,double Distance,ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Utilities")
    static FVector GetRandomPointInRadius(FVector Location,float Radius = 100);
    UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Utilities")
    static FVector GetRandomPointInBoxCollision(class UBoxComponent* BoxCollision);
    UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Utilities")
    static FVector GetRandomPointInSphereCollision(class USphereComponent* SphereCollision);
    
    UFUNCTION(BlueprintCallable)
    static void DebugLog(FString LogMessage,EEasylog LogVerbosity);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    static float GetDeltaSecondsFromStepMs(const float& StepMs);

    UFUNCTION(BlueprintCallable)
    static void AddReplicatedSubObject(UActorComponent* ActorComponent, UObject* Object, ELifetimeCondition LifetimeCondition = ELifetimeCondition::COND_None);
    UFUNCTION(BlueprintCallable)
    static void RemoveReplicatedSubObject(UActorComponent* ActorComponent, UObject* Object);
};