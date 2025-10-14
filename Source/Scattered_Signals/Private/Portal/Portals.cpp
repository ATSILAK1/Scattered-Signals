// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/Portals.h"

#include "DoorManager.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Scattered_Signals/Scattered_SignalsCharacter.h"

// Sets default values
APortals::APortals(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsActive = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent->Mobility = EComponentMobility::Static;

	PortalRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PortalRootComponent"));
	PortalRootComponent->SetupAttachment(GetRootComponent());
	PortalRootComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	PortalRootComponent->SetRelativeRotation(FRotator(0.0f,0.0f,0.0f));
	PortalRootComponent->Mobility = EComponentMobility::Movable;
}

bool APortals::IsActive()
{
	return bIsActive;
}

void APortals::SetActive(bool NewActive)
{
	bIsActive = NewActive;
}

AActor* APortals::GetTarget()
{
	return TargetPortal;
}

void APortals::SetTarget(AActor* NewTarget)
{
	TargetPortal = NewTarget;
}

bool APortals::IsPointInFrontOfPortal(FVector Point, FVector PortalLocation, FVector PortalNormal)
{
	FPlane PortalPlane = FPlane(PortalLocation, PortalNormal);
 	float PortalDot = PortalPlane.PlaneDot(Point);
 
 	return (PortalDot >= 0);
	
}


bool APortals::IsPointCrossingPortal(FVector Point, FVector PortalLocation, FVector PortalNormal)
{
	FVector IntesectionPoint;
	FPlane PortalPlane = FPlane(PortalLocation, PortalNormal);
	float PortalDot = PortalPlane.PlaneDot(Point);
	bool IsCrossing = false ;
	bool IsInFront = PortalDot >= 0 ;
	bool IsIntesect = FMath::SegmentPlaneIntersection(LastPositon ,Point,PortalPlane,IntesectionPoint );
	if (IsIntesect && !IsInFront && LastInFront)
		IsCrossing = true ;

	LastInFront = IsInFront;
	LastPositon = Point;
	return IsCrossing;
}
FVector APortals::ConvertLocationToActorSpace(FVector Location, AActor* Reference, AActor* TargetArg)
{
	if (Reference == nullptr || TargetArg == nullptr)
		return FVector::ZeroVector;

	FVector Direction = Location - Reference->GetActorLocation();
	FVector TargetLocation = TargetArg->GetActorLocation();

	FVector Dots;
	Dots.X = FVector::DotProduct(Direction, Reference->GetActorForwardVector());
	Dots.Y = FVector::DotProduct(Direction, Reference->GetActorRightVector());
	Dots.Z = FVector::DotProduct(Direction, Reference->GetActorUpVector());

	FVector NewDirection = Dots.X * TargetArg->GetActorForwardVector()
						  +Dots.Y * TargetArg->GetActorRightVector()
						  +Dots.Z * TargetArg->GetActorUpVector();

	return TargetLocation + NewDirection;
	
	
}

FRotator APortals::ConvertRotationToActorSpace(FRotator Rotation, AActor* Reference, AActor* TargetArg)
{
	if (Reference == nullptr || TargetArg == nullptr)
		return FRotator::ZeroRotator;

	FTransform SourceTransform = Reference->GetActorTransform();
	FTransform TargetTransform = TargetArg->GetActorTransform();
	FQuat QuatRotation = FQuat(Rotation);

	FQuat LocalQuat = SourceTransform.GetRotation().Inverse()*QuatRotation;
	FQuat NewWorldQuat = TargetTransform.GetRotation() * LocalQuat;
	return NewWorldQuat.Rotator();
}
// Function for the actor teleport 
void APortals::TeleportActor(AActor* ActorToTeleport)
{
	if (ActorToTeleport == nullptr || TargetPortal == nullptr)
		return;
	FVector SavedVelocity = FVector::ZeroVector;
	AScattered_SignalsCharacter* Character = nullptr;

	if (ActorToTeleport->IsA(AScattered_SignalsCharacter::StaticClass()))
	{
		Character = Cast<AScattered_SignalsCharacter>(ActorToTeleport);
		SavedVelocity = Character->GetVelocity();
	}
	FHitResult HitResult;
	FVector NewLocation = ConvertLocationToActorSpace(ActorToTeleport->GetActorLocation(), this , TargetPortal);
	ActorToTeleport->SetActorLocation(NewLocation,false , &HitResult,ETeleportType::TeleportPhysics);
	FRotator NewRotation = ConvertRotationToActorSpace(ActorToTeleport->GetActorRotation(), this , TargetPortal);
	ActorToTeleport->SetActorRotation(NewRotation);

	if (ActorToTeleport->IsA(AScattered_SignalsCharacter::StaticClass()))
	{

		APlayerController* PlayerController = Character->GetLocalViewingPlayerController();

		if (PlayerController != nullptr)
		{
			NewRotation = ConvertRotationToActorSpace(PlayerController->GetControlRotation(), this , TargetPortal);
			PlayerController->SetControlRotation(NewRotation);
		}
		{
			FVector Dots;
			Dots.X  = FVector::DotProduct( SavedVelocity, GetActorForwardVector() );
			Dots.Y  = FVector::DotProduct( SavedVelocity, GetActorRightVector() );
			Dots.Z  = FVector::DotProduct( SavedVelocity, GetActorUpVector() );
			FVector NewVelocity     = Dots.X * TargetPortal->GetActorForwardVector()
								   + Dots.Y * TargetPortal->GetActorRightVector()
								   + Dots.Z * TargetPortal->GetActorUpVector();

			Character->GetCharacterMovement()->Velocity = NewVelocity;
			
		}
	}
	LastPositon = NewLocation;
}

void APortals::ClearRTT_Implementation()
{
}

void APortals::SetRTT_Implementation(UTexture* RenderTexture)
{
}

void APortals::ForceTick_Implementation()
{
}

// Called when the game starts or when spawned
void APortals::BeginPlay()
{
	Super::BeginPlay();
	
	

}



// Called every frame
void APortals::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void APortals::TeleportToTheOtherPortal(AActor* Player)
{
auto PlayerPawn=Cast<APawn>(Player);
	Cast<APlayerController>(PlayerPawn->GetController());
}

void APortals::OnOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	
}


