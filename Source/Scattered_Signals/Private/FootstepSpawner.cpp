// Fill out your copyright notice in the Description page of Project Settings.


#include "FootstepSpawner.h"


#include "GameFramework/Actor.h"
#include "Components/DecalComponent.h"

#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UFootstepSpawner::UFootstepSpawner(): LastFootstepLocation(), FootstepMaterial(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFootstepSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		LastFootstepLocation = Owner->GetActorLocation();
	}

	FActorSpawnParameters SpawnParameters;
	
	FootstepParentActor = GetWorld()->SpawnActor<AActor>(
		AActor::StaticClass(),
		GetOwner()->GetActorLocation(),
		FRotator::ZeroRotator,
		SpawnParameters
	);

	if (FootstepParentActor)
	{
		USceneComponent* RootComponent = NewObject<USceneComponent>(FootstepParentActor);
		if (RootComponent)
		{
			RootComponent->RegisterComponent();
			FootstepParentActor->SetRootComponent(RootComponent);
		}
	}
	
	
}


// Called every frame
void UFootstepSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner()) return;

	FVector CurrentLocation = GetOwner()->GetActorLocation();
	float DistanceMoved = FVector::Dist(LastFootstepLocation, CurrentLocation);

	//Footsteps are placed depending on player movement,
	if (DistanceMoved >= StepDistance)
	{
		FRotator DecalRotation = FRotator(90.f, -90.0f, GetOwner()->GetActorRotation().Yaw);
		SpawnFootstepDecal(CurrentLocation, DecalRotation);
		LastFootstepLocation = CurrentLocation;
		
	}
}

void UFootstepSpawner::SpawnFootstepDecal(const FVector& Location, const FRotator& Rotation)
{
	if (!FootstepMaterial || !GetOwner()) return;

	FVector Start = Location+FVector(0, 0, 100.0f);
	FVector End = Location+FVector(0, 0, -200.0f);

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("FootstepTrace")), true, GetOwner());
	TraceParams.bReturnPhysicalMaterial = true;
	UDecalComponent* Decal;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams))
	{
		UE_LOG(LogTemp, Display, TEXT("HIT!!!"));
		
		FVector HitLocation = Hit.ImpactPoint;
		FRotator DecalRotation = FRotationMatrix::MakeFromXZ(
			GetOwner()->GetActorForwardVector(), // face the direction player is walking
			Hit.ImpactNormal                     // align with the slope
		).Rotator();
		DecalRotation.Roll = 90.0f;
		DecalRotation.Pitch = -90.0f;
		
		Decal = UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			FootstepMaterial,
			FVector(DecalSize),
			HitLocation,
			DecalRotation,
			DecalLifetime
		);

		if (FootstepMaterial)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepSound, HitLocation);
		}
	}else
	{
		Decal = UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			FootstepMaterial,
			FVector(DecalSize),
			Location,
			Rotation,
			0.f
		);
	}

	if (Decal && FootstepParentActor && FootstepParentActor->GetRootComponent())
	{
		
		Decal->AttachToComponent(FootstepParentActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	}
	
	
}

void UFootstepSpawner::Interact()
{
	for (auto Element : FootstepParentActor->Children)
	{
		Element->Destroy();
	}
}
