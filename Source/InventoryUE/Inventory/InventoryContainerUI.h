

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryContainerUI.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYUE_API UInventoryContainerUI : public UUserWidget
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Init a ContainerUI for any entity, like player inv, chest, etc
	/// </summary>
	/// <param name="_InventoryUI = Parent Class."></param>
	/// <param name="ID = ContainerID."></param>
	/// <param name="RowColumn = Rows and Columns count (Axis XY)."></param>
	/// <param name="Size = Real size unit for the UI Element."></param>
	/// <param name="_Padding = Padding between slots."></param>
	/// <param name="bShowSlotID = Show Index of the Slot."></param>
	void Build(class UInventoryComponent& Inventory, int ID, int RowCount, int ColumnCount, FVector2D Size, float _Padding, bool bShowSlotID);
	void SetPosition(FVector2D Pos);
	void Show(bool bValue);

protected:
	UPROPERTY(EditAnywhere)
	int CurrentItemSlotID = -1;
	UPROPERTY(EditAnywhere)
	int ContainerID = -1;

	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//class USizeBox* SizeBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UOverlay* OverlayPanel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UWrapBox* BoxContainer;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UImage* BackgroundImage;
	UPROPERTY(EditAnywhere)	TSubclassOf<class UItemSlotUI> BP_ItemSlotUI;

private:
	class UInventoryComponent* Inventory;
	class UCanvasPanelSlot* CanvasSlot;
};
