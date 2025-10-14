// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Portals.h"
#include "GameFramework/Actor.h"
#include "PortalManager.generated.h"

UCLASS()
class APortalManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalManager(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="Portal")
	void RequestTeleportByPortal(APortals* portal , AActor* TargetToTeleport);

	void SetControllerOwner(APlayerController* NewOwner);
	void Init();
	void Update(float DeltaTime);
	APortals* UpdatePortalsInWorld();
	void UpdateCapture(APortals* portal);
	ACharacter* Character;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FMatrix GetCameraProjectionMatrix();
	void GeneratePortalTexture();
	UPROPERTY()
	USceneCaptureComponent2D* SceneCapture;

	UPROPERTY(Transient)
	UTextureRenderTarget2D* PortalTexture;
	UPROPERTY()
	APlayerController* ControllerOwner;

	int32 PreviousScreenSizeX ;
	int32 PreviousScreenSizeY;
	float UpdateDelay ; 
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
