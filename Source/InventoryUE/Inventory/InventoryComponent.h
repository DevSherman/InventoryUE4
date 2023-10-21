#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "../Item/ItemStack.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYUE_API UInventoryComponent : public USceneComponent
{
	GENERATED_BODY()

public:

    UInventoryComponent();
    void ShowHideUI();
    bool IsOpened() { return bInventoryOpened; }
    void ResetInventory() { bInventoryOpened = false; }

    void SetCurrentItemSlotSelected(int ID);
    void SetCurrentContainerSelected(int ID);

    void RegisterItemSlotUI(class UItemSlotUI& ItemSlotUI);


protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

protected:
    UPROPERTY(EditAnywhere) TSubclassOf<class UInventoryUI> W_InventoryUI;
    UPROPERTY(EditAnywhere) FVector2D InventorySize = { 8, 4 };
    UPROPERTY(EditAnywhere) FVector2D ToolBarSize = { 8, 1 };
    UPROPERTY(EditAnywhere) float SlotUISize = 100.0f;
    UPROPERTY(EditAnywhere) float Padding = 1.0f;
    UPROPERTY(EditAnywhere) FVector2D ToolBarPos = { 0, 400 };
    UPROPERTY(EditAnywhere) FVector2D InventoryPos = { 0, 0 };

private:
    class APlayerController* PlayerController;
    class UInventoryUI* UI;

    TArray<class UItemSlotUI*> ItemSlotsUI;
    TArray<struct FItemStack> ItemStacks;

    //TArray<class UItemSlotUI*> ExternalItemSlotsUI;
    //TArray<struct FItemStack> ExternalItemStacks;

    bool bInventoryOpened = false;
    bool bOnSplitMode = false;

    int SelectedItemSlotID = -1; //current slot
    int CurrentContainerID = -1; //container
    int ItemSlotDragID = -1; //aux when drag event

    bool bMouseItemSlotVisible = false;
    bool bOutOfAreas = true;
};
