#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventoryUECharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;

UCLASS(config = Game)
class AInventoryUECharacter : public ACharacter
{
	GENERATED_BODY()

	//
	UPROPERTY(EditAnywhere)
	class UInteractionComponent* Interaction;
	UPROPERTY(EditAnywhere)
	class UInventoryComponent* Inventory;
	//

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	AInventoryUECharacter();

protected:
	virtual void Tick(float DeltaTime);
	virtual void BeginPlay();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

protected:

	void MoveForward(float Val);
	void MoveRight(float Val);
	void Interact();
	void InventoryUI();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

private:
	class APlayerController* Controller;
};

