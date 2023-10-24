#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemStack.h"
#include "ItemActor.generated.h"

UENUM()
enum class EItemType : uint8
{
	HARVESTABLE,
	PICKABLE,
};

UCLASS()
class AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemActor();

	void ShowOutline(bool Value, int Index = 2);
	void DestroyActor();

	FString GetDisplayName() { return DisplayName; }
	FString GetStringID() { return StringID; }

	FItemStack BuildStack()
	{
		FItemStack ItemStack;
		ItemStack.DisplayName = DisplayName;
		ItemStack.StringID = StringID;
		ItemStack.ItemActor = this;
		ItemStack.bCanStack = bCanStack;
		ItemStack.MaxStack = MaxStack;
		ItemStack.Count = Count;
		return ItemStack;
	}

	EItemType GetType() { return ItemType; }

protected:
	UPROPERTY(EditAnywhere, Category = "Item") class UStaticMeshComponent* Pivot;
	UPROPERTY(EditAnywhere, Category = "Item") class UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, Category = "Item") EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "ItemStack") FString DisplayName;
	UPROPERTY(EditAnywhere, Category = "ItemStack")	FString StringID;
	UPROPERTY(EditAnywhere, Category = "ItemStack")	bool bCanStack = false;
	UPROPERTY(EditAnywhere, Category = "ItemStack")	int MaxStack = 1;
	UPROPERTY(EditAnywhere, Category = "ItemStack")	int Count = 1;
};
