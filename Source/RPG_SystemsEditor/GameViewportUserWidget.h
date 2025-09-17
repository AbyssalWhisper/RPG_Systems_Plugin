#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "Components/PanelSlot.h"
#include "Engine/GameViewportClient.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SViewport.h"

#include "GameViewportUserWidget.generated.h"

class FGameViewportWidgetClient;
class SGameViewportWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameViewportObjectSpawned, AActor*, SpawnedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameViewportClicked, FVector, ClickLocation);

/**
 * Slot class para o GameViewportUserWidget
 */
UCLASS()
class RPG_SYSTEMSEDITOR_API UGameViewportUserWidgetSlot : public UPanelSlot
{
    GENERATED_BODY()

public:
    UGameViewportUserWidgetSlot(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
    TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
    TEnumAsByte<EVerticalAlignment> VerticalAlignment;

    virtual void SynchronizeProperties() override;
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

    void BuildSlot(TSharedRef<SOverlay> Overlay);

private:
    TSharedPtr<SBox> SlotBox;
};

/**
 * User Widget que contém uma viewport de jogo que pode ser usada em Blueprints e aceita um widget filho
 */
UCLASS(BlueprintType, Blueprintable)
class RPG_SYSTEMSEDITOR_API UGameViewportUserWidget : public UPanelWidget
{
    GENERATED_BODY()

public:
    UGameViewportUserWidget(const FObjectInitializer& ObjectInitializer);

    virtual void SynchronizeProperties() override;
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

    virtual UClass* GetSlotClass() const override;
    virtual void OnSlotAdded(UPanelSlot* InSlot) override;
    virtual void OnSlotRemoved(UPanelSlot* InSlot) override;

    UFUNCTION(BlueprintCallable, Category = "Panel")
    UWidget* GetContent() const;

    UFUNCTION(BlueprintCallable, Category = "Panel")
    void SetContent(UWidget* Content);

    UFUNCTION(BlueprintCallable, Category = "Panel")
    void ClearContent();

    // Funções de gameplay
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void StartGame();

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void PauseGame();

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void ResumeGame();

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void StopGame();

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    bool IsGameRunning() const;

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    bool IsGamePaused() const;

    // Funções de spawn
    UFUNCTION(BlueprintCallable, Category = "Viewport")
    void SpawnCube(FVector Location = FVector::ZeroVector, bool bEnablePhysics = true);

    UFUNCTION(BlueprintCallable, Category = "Viewport")
    void SpawnSphere(FVector Location = FVector::ZeroVector, bool bEnablePhysics = true);

    UFUNCTION(BlueprintCallable, Category = "Viewport")
    void SpawnCylinder(FVector Location = FVector::ZeroVector, bool bEnablePhysics = true);

    UFUNCTION(BlueprintCallable, Category = "Viewport")
    void SpawnCustomMesh(UStaticMesh* Mesh, FVector Location = FVector::ZeroVector, bool bEnablePhysics = true);

    UFUNCTION(BlueprintCallable, Category = "Viewport")
    void ClearAllObjects();

    UFUNCTION(BlueprintCallable, Category = "Viewport")
    void SetPhysicsEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Viewport")
    void SetCameraPosition(FVector Location, FRotator Rotation);

    UFUNCTION(BlueprintCallable, Category = "Viewport")
    int32 GetObjectCount() const;

    UFUNCTION(blueprintCallable,meta=(DeterminesOutputType ="ActorClass"), Category = "Viewport")
    AActor* SpawnActorByClass(TSubclassOf<AActor> ActorClass, FVector Location = FVector::ZeroVector,
                             FRotator Rotation = FRotator::ZeroRotator);

    // Propriedades editáveis
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Viewport Settings")
    bool bShowFloor = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Viewport Settings")
    bool bEnablePhysicsSimulation = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Viewport Settings")
    FVector InitialCameraLocation = FVector(400, 400, 300);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Viewport Settings")
    FRotator InitialCameraRotation = FRotator(-15, -45, 0);

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Viewport Events")
    FOnGameViewportObjectSpawned OnObjectSpawned;

    UPROPERTY(BlueprintAssignable, Category = "Viewport Events")
    FOnGameViewportClicked OnViewportClicked;

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;

private:
    TSharedPtr<SGameViewportWidget> ViewportWidget;
    UPROPERTY()
    UGameViewportClient* GameViewport;
    TArray<TWeakObjectPtr<AActor>> SpawnedActors;
    bool bIsGameRunning;
    bool bIsGamePaused;

    void InitializeViewport();
    void SpawnStaticMeshActor(UStaticMesh* Mesh, const FVector& Location, bool bEnablePhysics);
    FVector GetRandomSpawnLocation() const;
};

/**
 * Widget Slate personalizado para a viewport de jogo
 */
class SGameViewportWidget : public SViewport
{
public:
    SLATE_BEGIN_ARGS(SGameViewportWidget) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UGameViewportClient* InGameViewport, UGameViewportUserWidget* InOwnerWidget);

private:
    TWeakObjectPtr<UGameViewportClient> GameViewport;
    TWeakObjectPtr<UGameViewportUserWidget> OwnerWidget;
};
