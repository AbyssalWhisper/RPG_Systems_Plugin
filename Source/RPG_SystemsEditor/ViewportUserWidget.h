#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "Components/PanelSlot.h"
#include "AdvancedPreviewScene.h"
#include "EditorViewportClient.h"
#include "SEditorViewport.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"
#include "AssetViewerSettings.h"
#include "ViewportUserWidget.generated.h"

class FViewportWidgetClient;
class SViewportWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnViewportObjectSpawned, AActor*, SpawnedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnViewportClicked, FVector, ClickLocation);

/**
 * Slot class para o ViewportUserWidget
 */
UCLASS()
class RPG_SYSTEMSEDITOR_API UViewportUserWidgetSlot : public UPanelSlot
{
    GENERATED_BODY()

public:
    UViewportUserWidgetSlot(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
    TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
    TEnumAsByte<EVerticalAlignment> VerticalAlignment;

    // UPanelSlot interface
    virtual void SynchronizeProperties() override;
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

    void BuildSlot(TSharedRef<SOverlay> Overlay);

private:
    TSharedPtr<SBox> SlotBox;
};

/**
 * User Widget que contém uma viewport 3D que pode ser usada em Blueprints e aceita um widget filho
 */
UCLASS(BlueprintType, Blueprintable)
class RPG_SYSTEMSEDITOR_API UViewportUserWidget : public UPanelWidget
{
    GENERATED_BODY()

public:
    UViewportUserWidget(const FObjectInitializer& ObjectInitializer);

    // UWidget interface
    virtual void SynchronizeProperties() override;
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

    // UPanelWidget interface
    virtual UClass* GetSlotClass() const override;
    virtual void OnSlotAdded(UPanelSlot* InSlot) override;
    virtual void OnSlotRemoved(UPanelSlot* InSlot) override;

    // Funções para gerenciar o widget filho (similar ao UBorder)
    UFUNCTION(BlueprintCallable, Category = "Panel")
    UWidget* GetContent() const;

    UFUNCTION(BlueprintCallable, Category = "Panel")
    void SetContent(UWidget* Content);

    UFUNCTION(BlueprintCallable, Category = "Panel")
    void ClearContent();

    

    // Funções expostas para Blueprint
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

    UFUNCTION(blueprintCallable, Category = "Viewport")
    void SetViewportProfile(const int ProfileIndex)
    {
        if (PreviewScene.IsValid())
        {
            PreviewScene->SetProfileIndex(ProfileIndex);
        }
    }

    // Funções para gerenciar profiles do viewport (para combo box)
    UFUNCTION(BlueprintCallable, Category = "Viewport")
    TArray<FString> GetAllProfileNames() const;

    UFUNCTION(BlueprintCallable, Category = "Viewport")
    int32 GetCurrentProfileIndex() const;

    UFUNCTION(BlueprintCallable, Category = "Viewport")
    FString GetCurrentProfileName() const;

    UFUNCTION(BlueprintCallable, Category = "Viewport")
    void SetViewportProfileByName(const FString& ProfileName);

    UFUNCTION(BlueprintCallable, Category = "Viewport")
    int32 GetProfileCount() const;

    // Propriedades editáveis no Blueprint
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Viewport Settings")
    bool bShowFloor = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Viewport Settings")
    bool bEnablePhysicsSimulation = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Viewport Settings")
    FVector InitialCameraLocation = FVector(400, 400, 300);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Viewport Settings")
    FRotator InitialCameraRotation = FRotator(-15, -45, 0);

    // Configurações do Combo Box de Profiles
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profile Settings")
    bool bShowProfileComboBox = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profile Settings")
    FVector2D ProfileComboBoxPosition = FVector2D(10.0f, 10.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profile Settings")
    FVector2D ProfileComboBoxSize = FVector2D(200.0f, 30.0f);

    // Delegates para Blueprint
    UPROPERTY(BlueprintAssignable, Category = "Viewport Events")
    FOnViewportObjectSpawned OnObjectSpawned;

    UPROPERTY(BlueprintAssignable, Category = "Viewport Events")
    FOnViewportClicked OnViewportClicked;

    UFUNCTION(BlueprintCallable)
    void SpawnActorByClass(TSubclassOf<AActor> ActorClass, FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator);
protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;

private:
    TSharedPtr<SViewportWidget> ViewportWidget;
    TSharedPtr<FAdvancedPreviewScene> PreviewScene;
    TArray<TWeakObjectPtr<AActor>> SpawnedActors;

    // Combo box integrado para profiles
    TSharedPtr<SComboBox<TSharedPtr<FString>>> ProfileComboBox;
    TArray<TSharedPtr<FString>> ProfileOptions;

    void InitializeViewport();
    void SpawnStaticMeshActor(UStaticMesh* Mesh, const FVector& Location, bool bEnablePhysics);
    FVector GetRandomSpawnLocation() const;

    // Funções do combo box
    void InitializeProfileComboBox();
};

/**
 * Widget Slate personalizado para a viewport
 */
class SViewportWidget : public SEditorViewport
{
public:
    SLATE_BEGIN_ARGS(SViewportWidget) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, TSharedPtr<FAdvancedPreviewScene> InPreviewScene, UViewportUserWidget* InOwnerWidget);

protected:
    virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;

private:
    TSharedPtr<FAdvancedPreviewScene> PreviewScene;
    TSharedPtr<FViewportWidgetClient> ViewportClient;
    TWeakObjectPtr<UViewportUserWidget> OwnerWidget;
};

/**
 * Cliente da viewport personalizado
 */
class FViewportWidgetClient : public FEditorViewportClient
{
public:
    FViewportWidgetClient(FAdvancedPreviewScene* InPreviewScene, const TSharedRef<SViewportWidget>& InViewport, UViewportUserWidget* InOwnerWidget);

    // FEditorViewportClient interface
    virtual void Tick(float DeltaSeconds) override;
    virtual void Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
  
    void SetPhysicsEnabled(bool bEnabled);
    bool IsPhysicsEnabled() const { return bPhysicsEnabled; }

private:
    bool bPhysicsEnabled = true;
    TWeakObjectPtr<UViewportUserWidget> OwnerWidget;
};
