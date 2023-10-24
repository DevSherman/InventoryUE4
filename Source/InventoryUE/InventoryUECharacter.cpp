#include "InventoryUECharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

#include "Interaction/InteractionComponent.h"
#include "Item/ItemActor.h"
#include "Inventory/InventoryComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

AInventoryUECharacter::AInventoryUECharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));


	Interaction = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction"));
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
}

void AInventoryUECharacter::BeginPlay()
{
	Super::BeginPlay();
	Mesh1P->SetHiddenInGame(false, true);

	Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!Controller)
	{
		UE_LOG(LogTemp, Error, TEXT("Error to set PlayerController"));
		return;
	}
	Controller->SetInputMode(FInputModeGameOnly());
}

void AInventoryUECharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AInventoryUECharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AInventoryUECharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AInventoryUECharacter::Interact);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AInventoryUECharacter::InventoryUI);
}

void AInventoryUECharacter::Tick(float DeltaTime)
{
	Interaction->Update(FirstPersonCameraComponent);
}

void AInventoryUECharacter::Interact()
{
	if (Interaction->Avaliable())
	{
		AItemActor* Item = Interaction->GetItemActor();
		if (Item->GetType() == EItemType::PICKABLE)
		{
			if (Inventory->AddItem(Item))
			{
				Interaction->Clear();
				Item->Destroy();
			}
		}
	}
}

void AInventoryUECharacter::InventoryUI()
{
	Inventory->ShowHideUI();
}

void AInventoryUECharacter::MoveForward(float Value)
{
	if (Value != 0.0f) AddMovementInput(GetActorForwardVector(), Value);
}

void AInventoryUECharacter::MoveRight(float Value)
{
	if (Value != 0.0f) AddMovementInput(GetActorRightVector(), Value);
}
