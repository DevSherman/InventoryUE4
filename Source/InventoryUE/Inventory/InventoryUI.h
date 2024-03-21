#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Item/ItemStack.h"
#include "Types.h"
#include "InventoryUI.generated.h"

UCLASS()
class INVENTORYUE_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()

public:
    void Init(class UInventoryComponent& Inventory);
    void ShowMouseSlot(bool bValue);
    void UpdateMousePos();
    void UpdateMouseSlot(UTexture2D* Icon, int Count);

    inline class UInventoryContainerUI* GetInventory() { return InventoryContainerUI; }
    inline class UInventoryContainerUI* GetToolBar() { return ToolBarContainerUI; }

protected:
    virtual void NativeConstruct() override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
    virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:

    UPROPERTY(EditAnywhere) TSubclassOf<class UItemSlotUI> BP_ItemSlotUI;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UInventoryContainerUI* InventoryContainerUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UInventoryContainerUI* ToolBarContainerUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UItemSlotUI* MouseItemSlotUI;

    class UCanvasPanelSlot* MouseItemSlotUI_Slot; //mouse itemslot (panel slot)

private:
    class UInventoryComponent* Inventory;
    class UItemSlotUI* CurrentUISlot;
};
