// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "CoreMinimal.h"
#include "JsonBlueprintUtilities/Public/JsonBlueprintFunctionLibrary.h"
#include "JsonObjectConverter.h"
#include "ShaderCompiler.h"
#include "UObject/Object.h"

#include "Stats/StatsData.h"
#include "Stats/Stats.h"

#include "UObject/UnrealType.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "ShaderPipelineCache.h"

#include "BetterUtilitiesBPLibrary.generated.h"

class UReplicatedObject;
class UAttributeSet;

//É necessário implementar no arquivo cpp
DECLARE_LOG_CATEGORY_EXTERN(LogRPG_Systems, Log, All);

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
    Log,

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
        Max(1.0)
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

UENUM(BlueprintType)
enum class ERotationDirection : uint8
{
    Left, 
    Center,
    Right 
    
};

class UAbilitySystemComponent;
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

    UFUNCTION(BlueprintCallable,BlueprintPure)
    static ERotationDirection YawRotationDirection(FRotator BaseRotation, FRotator TargetRotation, float Angle);

    
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
    static float GetPercentFromAttribute(const UAbilitySystemComponent* AbilitySystemComponent,const FGameplayAttribute Attribute,const FGameplayAttribute MaxAttribute);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    static float GetAlphaPercentFromAttribute(const UAbilitySystemComponent* AbilitySystemComponent,const FGameplayAttribute Attribute,const FGameplayAttribute MaxAttribute);


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
    static void DebugLog(FString LogMessage, EEasylog LogVerbosity = EEasylog::Log, bool PrintScreen = false);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    static float GetDeltaSecondsFromStepMs(const float& StepMs);

    UFUNCTION(BlueprintCallable)
    static void AddReplicatedSubObject(UActorComponent* ActorComponent, UObject* Object, ELifetimeCondition LifetimeCondition = ELifetimeCondition::COND_None);
    UFUNCTION(BlueprintCallable)
    static void RemoveReplicatedSubObject(UActorComponent* ActorComponent, UObject* Object);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    static int32 GetNextIndex(int32 CurrentIndex, int32 ArraySize)
    {
        if (ArraySize == 0)
        {
            return INDEX_NONE; // Retorna INDEX_NONE se o array estiver vazio (convencionalmente -1 no Unreal Engine)
        }

        return (CurrentIndex + 1) % ArraySize; // Próximo índice, retorna 0 se for o último
    }
    UFUNCTION(BlueprintCallable, BlueprintPure)
    static int32 GetPreviousIndex(int32 CurrentIndex, int32 ArraySize)
    {
        if (ArraySize == 0)
        {
            return INDEX_NONE; // Retorna INDEX_NONE se o array estiver vazio
        }

        return (CurrentIndex - 1 + ArraySize) % ArraySize; // Volta para o último índice se estiver no início
    }
    UFUNCTION(BlueprintCallable, BlueprintPure)
    static FVector2D GetMainWindowPosition()
    {
        // Obtém a instância do aplicativo Slate
        TSharedPtr<SWindow> MainWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
    
        if (MainWindow.IsValid())
        {
            // Obtém a posição da janela
            FVector2D WindowPosition = MainWindow->GetPositionInScreen();
            return WindowPosition;
        }

        // Se a janela não estiver disponível, retorna um vetor nulo
        return FVector2D::ZeroVector;
    }
    UFUNCTION(BlueprintCallable, BlueprintPure)
    static FVector2D GetViewportPosition()
    {
        // Obtém a instância do aplicativo Slate
        TSharedPtr<SWindow> MainWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
    
        if (MainWindow.IsValid())
        {
            // Obtém a posição da janela
            FVector2D WindowPosition = MainWindow->GetPositionInScreen() + (MainWindow->GetSizeInScreen() - MainWindow->GetClientSizeInScreen());
            return WindowPosition;
        }

        // Se a janela não estiver disponível, retorna um vetor nulo
        return FVector2D::ZeroVector;
    }

    
    template<typename T>
    static void LoadAssetAsync(TSoftObjectPtr<T> AssetPtr, TFunction<void(T*)> OnLoaded);
    
    
    UFUNCTION(BlueprintCallable, Category = "Utilities")
    static FString ObjectToJsonString(UObject* SourceObject,bool bOnlySaveGame = true)
    {
        FJsonObjectWrapper JsonObjectWrapper = ObjectToJsonObject(SourceObject, bOnlySaveGame);

        if (!JsonObjectWrapper.JsonObject.IsValid())
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to convert object to JSON."));
            return TEXT("{}");
        }

        FString JsonString;
        JsonObjectWrapper.JsonObjectToString(JsonString);
        return JsonString;
    }

    UFUNCTION(BlueprintCallable, Category = "Utilities")
    static FJsonObjectWrapper ObjectToJsonObject(UObject* SourceObject, bool bOnlySaveGame = true)
    {
        FJsonObjectWrapper JsonObjectWrapper;

        if (!SourceObject)
        {
            UE_LOG(LogTemp, Warning, TEXT("SourceObject is null."));
            return JsonObjectWrapper;
        }

        // Criar um objeto JSON para armazenar os dados
        TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

        // Obter a classe do objeto
        UClass* ObjectClass = SourceObject->GetClass();

        // Iterar sobre todas as propriedades do objeto
        for (TFieldIterator<FProperty> PropIt(ObjectClass); PropIt; ++PropIt)
        {
            FProperty* Property = *PropIt;

            if (!Property)
            {
                continue;
            }

            // Se bOnlySaveGame for true, verificar a flag SaveGame
            if (bOnlySaveGame && !Property->HasAllPropertyFlags(CPF_SaveGame))
            {
                continue; // Ignorar propriedades que não têm a flag SaveGame
            }

            // Obter o valor bruto da propriedade
            const void* ValuePtr = Property->ContainerPtrToValuePtr<void>(SourceObject);

            if (!ValuePtr)
            {
                continue;
            }

            // Converter a propriedade para um valor JSON
            TSharedPtr<FJsonValue> JsonValue = FJsonObjectConverter::UPropertyToJsonValue(Property, ValuePtr);

            if (JsonValue.IsValid())
            {
                // Adicionar o campo ao objeto JSON
                JsonObject->SetField(Property->GetName(), JsonValue);
            }
        }

        // Atribuir o objeto JSON ao wrapper
        JsonObjectWrapper.JsonObject = JsonObject;

        return JsonObjectWrapper;
    }

    UFUNCTION(BlueprintCallable, Category = "Utilities")
    static bool SaveObjectToJsonFile(UObject* SourceObject,bool bOnlySaveGame = true,FString FilePath = "")
    {
        FJsonObjectWrapper JsonObjectWrapper = ObjectToJsonObject(SourceObject, bOnlySaveGame);

        if (!JsonObjectWrapper.JsonObject.IsValid())
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to convert object to JSON."));
            return false;
        }
        FFilePath FilePathStruct;
        FilePathStruct.FilePath = FilePath;
        return UJsonBlueprintFunctionLibrary::ToFile(JsonObjectWrapper,FilePathStruct);
    }

    UFUNCTION(BlueprintCallable,meta = (  WorldContext="WorldContextObject",DeterminesOutputType = "ObjectClass"), Category = "Utilities")
    static UObject* ObjectFromJsonString(UObject* WorldContextObject,TSubclassOf<UObject> ObjectClass,const FString& JsonString)
    {
        FJsonObjectWrapper JsonObjectWrapper = FJsonObjectWrapper();
        
        if (JsonObjectWrapper.JsonObjectFromString(JsonString))
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to parse JSON string."));
            return nullptr;
        }
        
        return ObjectFromJsonObject(WorldContextObject, ObjectClass, JsonObjectWrapper);
    }

    UFUNCTION(BlueprintCallable,meta = (  WorldContext="WorldContextObject",DeterminesOutputType = "ObjectClass"), Category = "Utilities")
    static UObject* ObjectFromJsonObject(UObject* WorldContextObject,TSubclassOf<UObject> ObjectClass, FJsonObjectWrapper& OutJsonObject)
    {
        UObject* TargetObject = NewObject<UObject>(WorldContextObject, ObjectClass);
        
        if (!TargetObject)
        {
            UE_LOG(LogTemp, Warning, TEXT("TargetObject is null."));
            return nullptr;
        }


        // Iterar sobre as propriedades do objeto
        for (TFieldIterator<FProperty> PropIt(ObjectClass); PropIt; ++PropIt)
        {
            FProperty* Property = *PropIt;

            if (!Property)
            {
                continue;
            }

            // Obter o valor do JSON com o mesmo nome da propriedade
            TSharedPtr<FJsonValue> JsonValue = OutJsonObject.JsonObject->TryGetField(Property->GetName());
            if (!JsonValue.IsValid())
            {
                continue;
            }

            // Obter o ponteiro para o valor da propriedade no objeto
            void* ValuePtr = Property->ContainerPtrToValuePtr<void>(TargetObject);
            if (!ValuePtr)
            {
                continue;
            }

            // Definir o valor da propriedade com base no JSON
            if (!FJsonObjectConverter::JsonValueToUProperty(JsonValue, Property, ValuePtr))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to set property '%s' from JSON."), *Property->GetName());
            }
        }

        return TargetObject;
    }

    UFUNCTION(BlueprintCallable,meta = (  WorldContext="WorldContextObject",DeterminesOutputType = "ObjectClass"), Category = "Utilities")
    static UObject* LoadObjectFromJsonFile(UObject* WorldContextObject,TSubclassOf<UObject> ObjectClass,FString FilePath)
    {
        FJsonObjectWrapper JsonObjectWrapper = FJsonObjectWrapper();
        FFilePath FilePathStruct;
        FilePathStruct.FilePath = FilePath;
        UJsonBlueprintFunctionLibrary::FromFile(FilePathStruct,JsonObjectWrapper);
        return ObjectFromJsonObject(WorldContextObject, ObjectClass, JsonObjectWrapper);
    }

    UFUNCTION(BlueprintCallable,meta = ( DeterminesOutputType = "ObjectClass"), Category = "Utilities")
    static UObject* SetObjectVariablesFromJsonFile(UObject* Target,FString FilePath)
    {
        if (!Target)return nullptr;
        FJsonObjectWrapper JsonObjectWrapper = FJsonObjectWrapper();
        FFilePath FilePathStruct;
        FilePathStruct.FilePath = FilePath;
        UJsonBlueprintFunctionLibrary::FromFile(FilePathStruct,JsonObjectWrapper);
       
        // Iterar sobre as propriedades do objeto
        for (TFieldIterator<FProperty> PropIt(Target->GetClass()); PropIt; ++PropIt)
        {
            FProperty* Property = *PropIt;

            if (!Property)
            {
                continue;
            }

            // Obter o valor do JSON com o mesmo nome da propriedade
            TSharedPtr<FJsonValue> JsonValue = JsonObjectWrapper.JsonObject->TryGetField(Property->GetName());
            if (!JsonValue.IsValid())
            {
                continue;
            }

            // Obter o ponteiro para o valor da propriedade no objeto
            void* ValuePtr = Property->ContainerPtrToValuePtr<void>(Target);
            if (!ValuePtr)
            {
                continue;
            }

            // Definir o valor da propriedade com base no JSON
            if (!FJsonObjectConverter::JsonValueToUProperty(JsonValue, Property, ValuePtr))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to set property '%s' from JSON."), *Property->GetName());
            }
        }
        return Target;
        
    }
    
    UFUNCTION(BlueprintCallable,meta=(WorldContext="WorldContextObject"))
    static void TravelToMapByName(UObject* WorldContextObject, const FString& MapName, const FString& Options = "")
    {
        if (!WorldContextObject || !WorldContextObject->GetWorld())
        {
            UE_LOG(LogTemp, Warning, TEXT("WorldContextObject or World is null."));
            return;
        }
        UKismetSystemLibrary::ExecuteConsoleCommand(WorldContextObject, "ServerTravel "+ MapName + "?listen");
    }

    UFUNCTION(BlueprintCallable,meta=(WorldContext="WorldContextObject"))
    static void TravelToMapByReference(UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Level, const FString& Options = "")
    {
        if (!WorldContextObject || !WorldContextObject->GetWorld())
        {
            UE_LOG(LogTemp, Warning, TEXT("WorldContextObject or World is null."));
            return;
        }
        UKismetSystemLibrary::ExecuteConsoleCommand(WorldContextObject, "ServerTravel "+ Level.GetAssetName() + "?listen");
    }
    
    UFUNCTION(BlueprintCallable)
    static TArray<UClass*> GetAllSubclassesOf(UClass* BaseClass);
    UFUNCTION(BlueprintCallable)
    static TArray<UClass*> GetAllSubclassesOfFromAssetRegistry(UClass* BaseClass, TArray<FString> PathsToScan);
    UFUNCTION(BlueprintCallable, Category = "Utilities")
    static TArray<UClass*> GetBlueprintClassesOfParent(UClass* ParentClass);

    //get asset class from assetdata
    UFUNCTION(blueprintCallable, Category = "Utilities")
    static UClass* GetAssetClassFromAssetData(const FAssetData& AssetData);

    
    // getuniqueid
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Utilities")
    static int32 GetUniqueId(const UObject* Object)
    {
        if (!Object)
        {
            return Object->GetUniqueID();
        }
        return 0; 
    }
    
    // IsInGameThread
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Utilities")
    static bool IsInGameThread_BP()
    {
        return IsInGameThread();
    }
};

template<typename T>
inline void UBetterUtilities::LoadAssetAsync(TSoftObjectPtr<T> AssetPtr, TFunction<void(T*)> OnLoaded)
{
    if (AssetPtr.IsValid())
    {
        // Se o asset já estiver carregado, executa a função imediatamente
        OnLoaded(AssetPtr.Get());
        return;
    }

    // Obtém o StreamableManager para gerenciar o carregamento assíncrono
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

    // Carrega o asset assíncronamente
    Streamable.RequestAsyncLoad(AssetPtr.ToSoftObjectPath(), [AssetPtr, OnLoaded]()
        {
            // Verifica se o asset foi carregado com sucesso
            if (AssetPtr.IsValid())
            {
                UBetterUtilities::DebugLog(FString::Format(TEXT("Asset Loaded: {0}"), { AssetPtr.GetAssetName()}), EEasylog::Log);

                // Executa o lambda passando o asset carregado
                OnLoaded(AssetPtr.Get());
            }
            else
            {
                UBetterUtilities::DebugLog(TEXT("Failed to load asset."), EEasylog::Warning);
            }
        });
    
}


