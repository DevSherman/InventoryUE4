#include "ItemActor.h"

AItemActor::AItemActor()
{
	RootComponent = Pivot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pivot"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetRenderCustomDepth(true);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void AItemActor::ShowOutline(bool Value, int Index)
{
	Mesh->SetCustomDepthStencilValue(Value ? Index : 0);
}

void AItemActor::DestroyActor()
{
	Destroy();
}

