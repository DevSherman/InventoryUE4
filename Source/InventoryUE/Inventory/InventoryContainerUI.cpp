
#include "InventoryContainerUI.h"
#include "Components/WrapBox.h"
#include "Components/Overlay.h"
#include "Components/CanvasPanelSlot.h"
#include "ItemSlotUI.h"
#include "InventoryComponent.h"
#include "Utls.h"

void UInventoryContainerUI::Build(UInventoryComponent& _Inventory, int ID, int RowCount, int ColumnCount, FVector2D Size, float _Padding, bool bShowSlotID)
{
	this->Inventory = &_Inventory;
	ContainerID = ID;

	BP_ItemSlotUI = Utls::LoadBlueprintFromPath<UItemSlotUI>("WidgetBlueprint'/Game/Widgets/W_ItemSlotUI.W_ItemSlotUI'");
	if (!BP_ItemSlotUI) return;

	BoxContainer->SetInnerSlotPadding(FVector2D(_Padding, _Padding));
	UE_LOG(LogTemp, Warning, TEXT("[UInventoryContainerUI] InnerSlotPadding: (%d-%d)"), BoxContainer->InnerSlotPadding.X, BoxContainer->InnerSlotPadding.Y);

	//SetDesiredSizeInViewport(Size);

	CanvasSlot = Cast<UCanvasPanelSlot>(this->Slot);
	if (CanvasSlot) CanvasSlot->SetSize(Size);
	else { UE_LOG(LogTemp, Error, TEXT("[UInventoryContainerUI] CanvasSlot not found in parent Widget")); return; }

	//UE_LOG(LogTemp, Warning, TEXT("CanvasSlotSize: %d %d"), GetDesiredSize().X, GetDesiredSize().Y);

	for (int slotID = 0; slotID < (RowCount * ColumnCount); slotID++)
	{
		UItemSlotUI* ItemSlotUI = CreateWidget<UItemSlotUI>(GetWorld(), BP_ItemSlotUI);
		ItemSlotUI->Init(*Inventory, slotID, bShowSlotID);

		BoxContainer->AddChild(ItemSlotUI);
		Inventory->RegisterItemSlotUI(*ItemSlotUI);
	}

	UE_LOG(LogTemp, Warning, TEXT("[UInventoryContainerUI] ContainerID: %d  builded."), ContainerID);
}

void UInventoryContainerUI::SetPosition(FVector2D Pos)
{
	CanvasSlot->SetPosition(Pos);
}

void UInventoryContainerUI::Show(bool bValue)
{
	this->SetVisibility(bValue ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
