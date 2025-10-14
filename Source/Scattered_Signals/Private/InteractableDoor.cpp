// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoor.h"

#include "DoorManager.h"

// Sets default values
AInteractableDoor::AInteractableDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractableDoor::BeginPlay()
{
	Super::BeginPlay();

	OnDoorStateChanged.AddDynamic(this, &AInteractableDoor::Interact);
	//DoorState = DoorState::Closed;
	SkeletalMesh =  FindComponentByClass<USkeletalMeshComponent>();
	SkeletalMesh->SetMaterial(0,DoorMaterial);
	SkeletalMesh->SetPlayRate(5);
	
}

// Called every frame
void AInteractableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableDoor::Interact()
{
	
	ADoorManager::SwitchDoorStateDelegate.Broadcast();
	
	
}
void AInteractableDoor::ChangeTheDoorState()
{
	if (DoorState == DoorState::Closed && !IsDisabledForTrapRoom)
	{
		DoorState = DoorState::Open;
		
	}
	else
	{
		
		DoorState = DoorState::Closed;
	}
	
		
	

}

void AInteractableDoor::CloseDoor()
{
	DoorState = DoorState::Closed;
}

void AInteractableDoor::ChangeTrapRoomState()
{
	SkeletalMesh->SetMaterial(1,DoorMaterialIfTrapRoom);
}

void AInteractableDoor::InitializeDoor()
{
	SkeletalMesh->SetMaterial(0,DoorMaterial);
}

