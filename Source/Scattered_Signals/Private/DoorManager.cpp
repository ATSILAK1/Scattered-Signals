// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorManager.h"

#include "EngineUtils.h"

// Sets default values
ADoorManager::ADoorManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FSwitchDoorStateDelegate ADoorManager::SwitchDoorStateDelegate;
// Called when the game starts or when spawned
void ADoorManager::BeginPlay()
{
	Super::BeginPlay();
	SwitchDoorStateDelegate.AddDynamic(this,&ADoorManager::SwitchDoorState);
	for (TActorIterator<AInteractableDoor> it(GetWorld()) ; it; ++it)
	{
		DoorList.Add(*it);
	}
}

// Called every frame
void ADoorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ADoorManager::SwitchDoorState()
{
	
	for (AInteractableDoor* Element : DoorList)
	{
		Element->ChangeTheDoorState();
	}
}

