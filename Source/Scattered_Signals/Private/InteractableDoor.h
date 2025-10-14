// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"


#include "InteractableDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorStateChanged);
UENUM(BlueprintType)
enum class DoorState : uint8 { Closed = 0  UMETA(DisplayName = "Closed") , Open = 1  UMETA(DisplayName = "Open")};
UCLASS()
class AInteractableDoor : public AActor, public IInteractable 
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AInteractableDoor();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	DoorState DoorState;

	UPROPERTY(EditAnywhere)
	FOnDoorStateChanged OnDoorStateChanged;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDisabledForTrapRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsTrapRoom ; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* DoorMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* DoorMaterialIfTrapRoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMesh;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact() override;
	void ChangeTheDoorState();
	void CloseDoor();
	void ChangeTrapRoomState();
	void InitializeDoor();
};
