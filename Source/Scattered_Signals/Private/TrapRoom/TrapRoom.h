// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableDoor.h"
#include "GameFramework/Actor.h"
#include "TrapRoom.generated.h"

class USphereComponent;
class UBoxComponent;

UCLASS()
class ATrapRoom : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATrapRoom();
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxForThePlayerCollision;
	// this will take in condieration only the player in movement throught it 

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereForTheDoorCollision; // this will gather all the doors on its perimeter 

	UPROPERTY(EditAnywhere)
	TArray<AInteractableDoor*> Doors;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);

	UFUNCTION()
	void OnOverLapBeginFortheDoors(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                               const FHitResult& SweepResult);

	void InitTheDoorsArrayWithTheOverlapingDoors();
};
