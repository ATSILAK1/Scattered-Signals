// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableObject/PushableObjts.h"


#include "Engine/StaticMeshActor.h"

// Sets default values
FMoveTheOtherObject APushableObjts::MoveTheOtherObject;

APushableObjts::APushableObjts()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);

	BoxEast = CreateDefaultSubobject<UBoxComponent>("Box East");
	BoxEast->AddLocalOffset(FVector(0.0f, -60.0f, 0.0f));

	BoxNorth = CreateDefaultSubobject<UBoxComponent>("Box North");
	BoxNorth->AddLocalOffset(FVector(60.0f, 0, 0.0f));

	BoxSouth = CreateDefaultSubobject<UBoxComponent>("Box South");
	BoxSouth->AddLocalOffset(FVector(-60.0f, 0, 0.0f));

	BoxWest = CreateDefaultSubobject<UBoxComponent>("Box West");
	BoxWest->AddLocalOffset(FVector(0.0f, 60.0f, 0.0f));

	BoxNorth->SetupAttachment(StaticMesh);
	BoxEast->SetupAttachment(StaticMesh);
	BoxSouth->SetupAttachment(StaticMesh);
	BoxWest->SetupAttachment(StaticMesh);


	BoxNorth->OnComponentBeginOverlap.AddDynamic(this, &APushableObjts::APushableObjts::OnOverlapBeginForNorth);
	BoxSouth->OnComponentBeginOverlap.AddDynamic(this, &APushableObjts::APushableObjts::OnOverlapBeginForSouth);
	BoxEast->OnComponentBeginOverlap.AddDynamic(this, &APushableObjts::OnOverlapBeginForEast);
	BoxWest->OnComponentBeginOverlap.AddDynamic(this, &APushableObjts::OnOverlapBeginForWest);
}

// Called when the game starts or when spawned
void APushableObjts::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
}

// Called every frame
void APushableObjts::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APushableObjts::Interact()
{
	Move();
}


void APushableObjts::Move()
{
	FVector PushVector = FVector(0.0f, 0.0f, 0.0f);;
	if (SideToPush == FaceFacing::North)
		PushVector = FVector(-MoveDistance, 0.0f, 0.0f);
	else if (SideToPush == FaceFacing::East)
		PushVector = FVector(0, MoveDistance, 0.0f);
	else if (SideToPush == FaceFacing::South)
		PushVector = FVector(MoveDistance, 0.0f, 0.0f);
	else if (SideToPush == FaceFacing::West)
		PushVector = FVector(0, -MoveDistance, 0.0f);

	FHitResult Hit;
	FVector StartLocation = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);
	FVector End = StartLocation + PushVector;

	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, End, ECollisionChannel::ECC_Visibility);
	if (Hit.bBlockingHit) { return; }

	AddActorLocalOffset(PushVector);
	MoveTheOtherObject.Broadcast(PushVector, this);
	
}

void APushableObjts::Move(FVector Move)
{
	//StaticMesh->AddImpulse(Move);
	FHitResult Hit;
	FVector Start = GetActorLocation() + FVector3d(0, 0, 50);
	FVector End = Start + Move;

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility);
	if (Hit.bBlockingHit) { return; }
	AddActorLocalOffset(Move);
}


void APushableObjts::OnOverlapBeginForNorth(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		SideToPush = FaceFacing::North;
		UE_LOG(LogTemp, Warning, TEXT("Overlap North Begin with: %s (Component: %s)"),
		       *OtherActor->GetName(),
		       *OtherComp->GetName());
	}
	if (OtherActor->IsA(AStaticMeshActor::StaticClass()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("North move is impossible"));
		CanGoNorth = false;
	}
}

void APushableObjts::OnOverlapBeginForSouth(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		SideToPush = FaceFacing::South;
		UE_LOG(LogTemp, Warning, TEXT("Overlap South Begin  with: %s (Component: %s)"),
		       *OtherActor->GetName(),
		       *OtherComp->GetName());
	}
	if (OtherActor->IsA(AStaticMeshActor::StaticClass()))
	{
		CanGoSouth = false;
		UE_LOG(LogTemp, Warning, TEXT("South move is impossible"));
	}
}

void APushableObjts::OnOverlapBeginForWest(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		SideToPush = FaceFacing::West;
		UE_LOG(LogTemp, Warning, TEXT("Overlap West Begin with: %s (Component: %s)"),
		       *OtherActor->GetName(),
		       *OtherComp->GetName());
	}

	if (OtherActor->IsA(AStaticMeshActor::StaticClass()))
	{
		CanGoWest = false;
		UE_LOG(LogTemp, Warning, TEXT("West move is impossible"));
	}
}

void APushableObjts::OnOverlapBeginForEast(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		SideToPush = FaceFacing::East;
		UE_LOG(LogTemp, Warning, TEXT("Overlap East Begin with: %s (Component: %s)"),
		       *OtherActor->GetName(),
		       *OtherComp->GetName());
	}

	if (OtherActor->IsA(AStaticMeshActor::StaticClass()))
	{
		CanGoEast = false;
		UE_LOG(LogTemp, Warning, TEXT("East move is impossible"));
	}
}

/*
*/
