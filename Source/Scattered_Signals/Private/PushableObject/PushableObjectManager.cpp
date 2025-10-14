// Fill out your copyright notice in the Description page of Project Settings.
#include "PushableObject/PushableObjectManager.h"
#include "EngineUtils.h"

// Sets default values
APushableObjectManager::APushableObjectManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void APushableObjectManager::BeginPlay()
{
	Super::BeginPlay();
	for (TActorIterator<APushableObjts> it(GetWorld()) ; it; ++it)
	{
		PushableObjects.Add(*it);
	}

	APushableObjts::MoveTheOtherObject.AddDynamic(this,&APushableObjectManager::MoveThemAll);
}
// Called every frame
void APushableObjectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void APushableObjectManager::MoveThemAll(FVector Move , APushableObjts* Actor)
{

	for (auto element : PushableObjects)
	{
		if (Actor == element) continue;

		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, Move.ToString() +" Move Them ALl "  );
		
		element->Move(Move);
	}
	
}

