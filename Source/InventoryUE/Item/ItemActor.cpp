#include "ItemActor.h"

AItemActor::AItemActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
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

