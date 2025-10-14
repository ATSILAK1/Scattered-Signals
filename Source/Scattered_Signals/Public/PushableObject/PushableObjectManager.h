// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PushableObjts.h"
#include "GameFramework/Actor.h"
#include "PushableObjectManager.generated.h"

UCLASS()
class SCATTERED_SIGNALS_API APushableObjectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APushableObjectManager();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<APushableObjts*> PushableObjects;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MovementSpeed = 10 ; 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void MoveThemAll(FVector Move,APushableObjts* Actor );
};
