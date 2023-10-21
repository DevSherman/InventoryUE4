#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Item/ItemStack.h"
#include "InventoryUI.generated.h"


UCLASS()
class INVENTORYUE_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()

public:
    void Init(class UInventoryComponent& Inventory);
    void ShowMouseSlot(bool bValue);
    void UpdateMousePos(FVector2D Pos);

    class UInventoryContainerUI* GetInventory();
    class UInventoryContainerUI* GetToolBar();


protected:
    virtual void NativeConstruct() override;
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

protected:

    UPROPERTY(EditAnywhere) TSubclassOf<class UItemSlotUI> BP_ItemSlotUI;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UInventoryContainerUI* InventoryContainerUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UInventoryContainerUI* ToolBarContainerUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UItemSlotUI* MouseItemSlotUI;

    class UCanvasPanelSlot* MouseItemSlotUI_Slot; //mouse itemslot (panel slot)
    struct FItemStack MouseItemStack = FItemStack();

private:
    class UInventoryComponent* Inventory;

    class UItemSlotUI* CurrentUISlot;
};
