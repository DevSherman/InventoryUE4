#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "../Item/ItemStack.h"
#include "Types.h"
#include "InventoryComponent.generated.h"

enum EMoveType
{
    INCREASE,
    DECREASE
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYUE_API UInventoryComponent : public USceneComponent
{
	GENERATED_BODY()

public:

    UInventoryComponent();
    void ShowHideUI();
    bool IsOpened() const { return bInventoryOpened; }
    void ResetInventory() { bInventoryOpened = false; }

    void UpdateCurrentItemSlotSelected(int ID);
    inline void UpdateCurrentContainerSelected(int ID) { CurrentContainerID = ID; }

    void RegisterItemSlotUI(class UItemSlotUI& ItemSlotUI);
    bool AddItem(class AItemActor* ItemActor);

    void OnMouseButtonPressed(EMouseButton button);
    void OnMouseButtonRelease(EMouseButton button);

    TSubclassOf<class UItemSlotUI> GetItemSlotUI() const;

private:
    UTexture2D* LoadIcon(FString StringID);
    bool CheckAvaliableSlot(struct FItemStack ItemStack);
    void SetItemStackSlot(int SlotID, FItemStack ItemStack);
    void UpdateItemStackCount(int SlotID, int Count);

    //mouse button events
    void LeftClickEvent();
    void MiddleClickEvent();
    void RightClickEvent();
    void ScrollEvent(EMoveType MoveType);

    void TryDropStackToSlot();
    void MoveStackToMouseSlot();
    void StackMouseSlotToCurrentSlot();
    void SetMouseSlot(FItemStack ItemStack);
    void ClearSlot(int ID);
    void ClearMouseSlot();
    void SwapSlots();
    void DropMouseSlotToCurrentSlot();
    void SplitCurrentStack();
    void SpreadMode();

    FVector2D SplitStack(int ItemStackCount, int SlotCount);

    //FItemStack GetCurrentItemStack() const { return ItemStackArray[CurrentItemSlotID]; }
    bool CurrentSlotHasItem() { return ItemStackArray[CurrentItemSlotID].HasItem(); }
    bool SlotsHasSameItem() { return ItemStackArray[CurrentItemSlotID].StringID == MouseItemStack.StringID; }

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    UPROPERTY(EditAnywhere) TSubclassOf<class UInventoryUI> W_InventoryUI;
    UPROPERTY(EditAnywhere)	TSubclassOf<class UItemSlotUI> BP_ItemSlotUI;
    UPROPERTY(EditAnywhere) FVector2D InventorySize = { 8, 4 };
    UPROPERTY(EditAnywhere) FVector2D ToolBarSize = { 8, 1 };
    UPROPERTY(EditAnywhere) float SlotUISize = 100.0f;
    UPROPERTY(EditAnywhere) float Padding = 1.0f;
    UPROPERTY(EditAnywhere) FVector2D ToolBarPos = { 0, 400 };
    UPROPERTY(EditAnywhere) FVector2D InventoryPos = { 0, 0 };
    UPROPERTY(EditAnywhere) FString IconsPath = "/Game/Textures/Icon/Item/";
    UPROPERTY(EditAnywhere) UTexture2D* IconNotFoundTexture;

private:
    class APlayerController* PlayerController;
    class UInventoryUI* UI;

    TArray<class UItemSlotUI*> ItemSlotUIArray;
    TArray<struct FItemStack> ItemStackArray;
    FItemStack MouseItemStack;

    //TArray<class UItemSlotUI*> ExternalItemSlotsUI;
    //TArray<struct FItemStack> ExternalItemStacks;

    bool bInventoryOpened = false;

    int CurrentItemSlotID = -1; //current slot
    int MouseSlotID_Cache = -1; //current aux slot id, used in swap events
    int CurrentContainerID = -1; //container

    TArray<int> SplitSlotIDArray; //middle button drag
    bool bOnSplitMode = false;
    int SplitItemStackCount = -1;

    bool bMouseHasItem = false;
    bool bOutOfAreas = true;
};
