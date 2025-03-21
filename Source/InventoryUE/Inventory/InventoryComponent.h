#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "../Item/ItemStack.h"
#include "Types.h"
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

    inline void SetCurrentItemSlotSelected(int ID);
    inline void SetCurrentContainerSelected(int ID);

    void RegisterItemSlotUI(class UItemSlotUI& ItemSlotUI);
    bool AddItem(class AItemActor* ItemActor);

    void OnClick(EMouseButton button);
    void OnClickRelease();

    TSubclassOf<class UItemSlotUI> GetItemSlotUI() const;

private:
    UTexture2D* LoadIcon(FString StringID);
    bool CheckAvaliableSlot(struct FItemStack ItemStack);
    void SetItemStackSlot(int SlotID, FItemStack ItemStack);
    void UpdateItemStackCount(int SlotID, int Count);

    void MoveStackToMouseSlot();
    void SetMouseSlot(FItemStack ItemStack);
    void ClearSlot();
    void ClearMouseSlot();
    void SwapSlots();
    void MoveCurrentStack();

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
    bool bOnSplitMode = false;

    int CurrentItemSlotID = -1; //current slot
    int CurrentContainerID = -1; //container
    
    int Cached_CurrentItemSlotID = -1; //current slot aux
    int Cached_CurrentContainerID = -1; //container aux

    bool bMouseHasItem = false;
    bool bOutOfAreas = true;
};
