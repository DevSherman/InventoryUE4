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

	FString GetDisplayName() { return Stack.DisplayName; }
	FString GetStringID() { return Stack.StringID; }
	FItemStack GetStack() { return Stack; }
	EItemType GetType() { return ItemType; }

protected:
	UPROPERTY(EditAnywhere, Category = "Item")
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, Category = "Item")
	EItemType ItemType;
	UPROPERTY(EditAnywhere, Category = "Item")
	struct FItemStack Stack;
};
