// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapRoom/TrapRoom.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Scattered_Signals/Scattered_SignalsCharacter.h"

// Sets default values
ATrapRoom::ATrapRoom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereForTheDoorCollision = CreateDefaultSubobject<USphereComponent>(TEXT("detection of the doors "));
	RootComponent = SphereForTheDoorCollision;

	SphereForTheDoorCollision->InitSphereRadius(400.0f);
	SphereForTheDoorCollision->SetGenerateOverlapEvents(true);
	SphereForTheDoorCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapRoom::OnOverLapBeginFortheDoors);
	BoxForThePlayerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("detection sphere"));
	BoxForThePlayerCollision->SetupAttachment(RootComponent);
	BoxForThePlayerCollision->InitBoxExtent(FVector(300));
	BoxForThePlayerCollision->SetGenerateOverlapEvents(true);
	BoxForThePlayerCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapRoom::OnOverLapBegin);
	BoxForThePlayerCollision->OnComponentEndOverlap.AddDynamic(this, &ATrapRoom::OnEndOverLap);
}

// Called when the game starts or when spawned
void ATrapRoom::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATrapRoom::InitTheDoorsArrayWithTheOverlapingDoors, 0.1f,
	                                false);
}

// Called every frame
void ATrapRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ATrapRoom::OnOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                               bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || !OtherComp)
	{
		return;
	}


	if (OtherActor->GetClass()->GetInheritanceSuper() == AScattered_SignalsCharacter::StaticClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Begin with: %s (Component: %s)"),
		       *OtherActor->GetName(),
		       *OtherComp->GetName());
		for (auto Element : Doors)
		{
			Element->CloseDoor();
			Element->SkeletalMesh->SetMaterial(0, Element->DoorMaterialIfTrapRoom);
			Element->IsDisabledForTrapRoom = true;
			Element->IsTrapRoom = true;

			//Element->ChangeTheDoorState();
		}
	}
}

void ATrapRoom::OnEndOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == this || !OtherComp)
	{
		return;
	}
	if (OtherActor->GetClass()->GetInheritanceSuper() == AScattered_SignalsCharacter::StaticClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap end with: %s (Component: %s)"),
		       *OtherActor->GetName(),
		       *OtherComp->GetName());
		for (auto Element : Doors)
		{
			Element->IsDisabledForTrapRoom = false;
			Element->SkeletalMesh->SetMaterial(0, Element->DoorMaterial);
		}
	}
}

void ATrapRoom::OnOverLapBeginFortheDoors(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
	// if (!OtherActor || OtherActor == this || !OtherComp)
	// 	return;
	// UE_LOG(LogTemp,Warning,TEXT("Overlap end with: %s") ,*OtherActor->GetName());
	//    		
	// if (OtherActor->Implements<UInteractable>())
	// {
	// 	Doors.Add(Cast<AInteractableDoor,AActor>(OtherActor) );
	// }
}

void ATrapRoom::InitTheDoorsArrayWithTheOverlapingDoors()
{
	TArray<AActor*> OverlappingActors;
	SphereForTheDoorCollision->GetOverlappingActors(OverlappingActors);
	for (auto Element : OverlappingActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Begin with: %s "),
		       *Element->GetName());

		if (Element->Implements<UInteractable>())
		{
			Doors.Add(Cast<AInteractableDoor, AActor>(Element));
		}
	}
}
