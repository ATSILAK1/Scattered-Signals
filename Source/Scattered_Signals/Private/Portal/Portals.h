// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Portals.generated.h"

UCLASS()
class APortals : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortals() = default;
	APortals(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintPure , Category="Portal")
	bool IsActive();
	UFUNCTION(BlueprintCallable , Category="Portal")
	void SetActive(bool NewActive);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable , Category="Portal")
	void ClearRTT();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable , Category="Portal")
	void SetRTT(UTexture* RenderTexture);
	UFUNCTION(BlueprintNativeEvent , Category="Portal")
	void ForceTick();
	UFUNCTION(BlueprintPure , Category="Portal")
	AActor* GetTarget();
	UFUNCTION(BlueprintCallable , Category="Portal")
	void SetTarget(AActor* NewTarget);
	UFUNCTION(BlueprintCallable , Category="Portal")
	bool IsPointInFrontOfPortal(FVector Point , FVector PortalLocation ,FVector PortalNormal );
	UFUNCTION(BlueprintCallable , Category="Portal")
	bool IsPointCrossingPortal(FVector Point , FVector PortalLocation ,FVector PortalNormal );
	UFUNCTION(BlueprintCallable , Category="Portal")
	void TeleportActor(AActor* ActorToTeleport);
	static FVector ConvertLocationToActorSpace(FVector Location, AActor* Reference , AActor* Target);
	static FRotator ConvertRotationToActorSpace(FRotator Rotation, AActor* Reference , AActor* Target);
                                                	
protected:

	UPROPERTY(BluePrintReadOnly , Category="Portal")
	USceneComponent* PortalRootComponent;

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool bIsActive;
	AActor* TargetPortal;
	FVector LastPositon;
	bool LastInFront;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TeleportToTheOtherPortal(AActor*);
	UFUNCTION()
	void OnOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

};
