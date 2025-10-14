// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PushableObjts.generated.h"

UENUM(BlueprintType)
enum class FaceFacing : uint8
{
	North = 0 UMETA(DisplayName = "north "), East = 1 UMETA(DisplayName = "East"),
	South = 2 UMETA(DisplayName = "south"), West = 3 UMETA(DisplayName = "West"), Not = 4 UMETA(DisplayName = "Not")};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMoveTheOtherObject, FVector, MoveVector, APushableObjts*, MovedObject);

UCLASS()
class SCATTERED_SIGNALS_API APushableObjts : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APushableObjts();

	static FMoveTheOtherObject MoveTheOtherObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=interactable)
	UBoxComponent* BoxNorth; // side number = 0 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=interactable)
	UBoxComponent* BoxSouth; // side number = 1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=interactable)
	UBoxComponent* BoxEast; // side number = 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=interactable)
	UBoxComponent* BoxWest; // side number = 3

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=interactable)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=interactable)
	bool CanGoNorth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=interactable)
	bool CanGoSouth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=interactable)
	bool CanGoEast;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=interactable)
	bool CanGoWest;

	FaceFacing SideToPush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=interactable)
	float MoveDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=interactable)
	FVector InitialLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact() override;
	void GetAllThePushableObjects();
	void Move();
	void Move(FVector Move);


	UFUNCTION()
	void OnOverlapBeginForNorth(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapBeginForSouth(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapBeginForWest(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapBeginForEast(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*
	UFUNCTION()
	void OnOverlapEndForNorth(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnOverlapEndForSouth(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnOverlapEndForWest(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnOverlapEndForEast(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
};
