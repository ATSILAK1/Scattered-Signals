// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interactable.h"
#include "FootstepSpawner.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCATTERED_SIGNALS_API UFootstepSpawner : public UActorComponent , public IInteractable
{
public:
	virtual void Interact() override;

private:
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFootstepSpawner();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector LastFootstepLocation;
	
	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	float StepDistance = 150.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	float DecalSize = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	float DecalLifetime = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	UMaterialInterface* FootstepMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	USoundBase* FootstepSound;

	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	AActor* FootstepParentActor;

	void SpawnFootstepDecal(const FVector& Location, const FRotator& Rotation);
	

		
};
