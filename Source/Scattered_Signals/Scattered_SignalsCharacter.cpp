// Copyright Epic Games, Inc. All Rights Reserved.

#include "Scattered_SignalsCharacter.h"
#include "Scattered_SignalsProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AScattered_SignalsCharacter

AScattered_SignalsCharacter::AScattered_SignalsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));


	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	PhysicsHandle->SetAngularDamping(1000.f);
	PhysicsHandle->SetLinearDamping(1000.f);
	PhysicsHandle->SetAngularDamping(2000.f);
	PhysicsHandle->SetAngularStiffness(2000.f);
	PhysicsHandle->SetInterpolationSpeed(50.f);
	

}

//////////////////////////////////////////////////////////////////////////// Input



void AScattered_SignalsCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AScattered_SignalsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AScattered_SignalsCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AScattered_SignalsCharacter::Look);

		EnhancedInputComponent->BindAction(GrabAction , ETriggerEvent::Started , this ,&AScattered_SignalsCharacter::GrabElement);
		EnhancedInputComponent->BindAction(GrabAction , ETriggerEvent::Completed , this ,&AScattered_SignalsCharacter::ReleaseElement);
		//EnhancedInputComponent->BindAction(ReleaselAction , ETriggerEvent:: , this ,&AScattered_SignalsCharacter::ReleaseElement);


	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AScattered_SignalsCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateGrabbedElement();
}

void AScattered_SignalsCharacter::TickActor(float DeltaTime, enum ELevelTick TickType,
	FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

 
     if( UGameplayStatics::GetPlayerController(GetWorld(), 0) != nullptr )
     {
         APlayerController* EPC = Cast<APlayerController>( UGameplayStatics::GetPlayerController(GetWorld(), 0) );
         //EPC->PortalManager->Update( DeltaTime );
     }
}

void AScattered_SignalsCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AScattered_SignalsCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X * CameraSensitivity);
		AddControllerPitchInput(LookAxisVector.Y * CameraSensitivity);
	}
}

void AScattered_SignalsCharacter::GrabElement() 
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation+ (GetActorForwardVector() * GrabDistance);
	UE_LOG(LogTemp,Warning,TEXT("%s - --- ---- -- %s") ,*StartLocation.ToString(), *EndLocation.ToString());
	UE_LOG(LogTemp,Warning,TEXT("the Forward Actor Vector %s") ,*GetActorForwardVector().ToString());
	
	FHitResult Hit;
	FCollisionQueryParams Parameters(FName(TEXT("HitResult")),false,GetOwner());

	GetWorld()->LineTraceSingleByObjectType(Hit,StartLocation,EndLocation,FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),Parameters);
//	DrawDebugDirectionalArrow(
//	GetWorld(),
//	StartLocation,
//	EndLocation ,
//	120.0f, // Arrow size
//	FColor::Cyan,
//	true, // Persistent
//	10,
//	0, // Depth priority
//	2.0f // Thickness
//);
	if (Hit.GetActor() != nullptr && Hit.GetComponent()->IsSimulatingPhysics())
	{
		UE_LOG(LogTemp,Warning,TEXT("this is the object hit by the rayCast %s "),*Hit.GetComponent()->GetName());
		PhysicsHandle->GrabComponentAtLocationWithRotation(Hit.GetComponent() , NAME_None , Hit.GetComponent()->GetCenterOfMass(),Hit.GetComponent()->GetComponentRotation());
	}
}

void AScattered_SignalsCharacter::ReleaseElement()
{
	if (PhysicsHandle != nullptr)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

void AScattered_SignalsCharacter::UpdateGrabbedElement()
{
	if (PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetActorLocation() + (GetActorForwardVector() * HoldDistance );
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetActorRotation());
	}
}
	