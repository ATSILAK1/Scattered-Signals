// Copyright Epic Games, Inc. All Rights Reserved.

#include "Scattered_SignalsPickUpComponent.h"

UScattered_SignalsPickUpComponent::UScattered_SignalsPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UScattered_SignalsPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UScattered_SignalsPickUpComponent::OnSphereBeginOverlap);
}

void UScattered_SignalsPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AScattered_SignalsCharacter* Character = Cast<AScattered_SignalsCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
