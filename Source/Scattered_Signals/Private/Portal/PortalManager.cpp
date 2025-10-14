// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/PortalManager.h"

#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortalManager::APortalManager(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PortalTexture = nullptr;
	UpdateDelay = 1.1f;
	PreviousScreenSizeX = 0 ;
	PreviousScreenSizeY = 0 ;

}

void APortalManager::RequestTeleportByPortal(APortals* portal, AActor* TargetToTeleport)
{
	if (portal != nullptr && TargetToTeleport != nullptr)
	{
		portal->TeleportActor(TargetToTeleport);
		APortals* FuturePortal = UpdatePortalsInWorld();
		if (FuturePortal != nullptr)
		{
			FuturePortal->ForceTick();
			UpdateCapture(FuturePortal);
		}
	}
}

void APortalManager::SetControllerOwner(APlayerController* NewOwner)
{
	ControllerOwner = NewOwner;
	Character = ControllerOwner->GetCharacter();
}

void APortalManager::GeneratePortalTexture()
{
	int32 CurrentSizeX = 1920;
    int32 CurrentSizeY = 1080;

    if( ControllerOwner != nullptr )
    {
        ControllerOwner->GetViewportSize(CurrentSizeX, CurrentSizeY);
    }

    // Use a smaller size than the current 
    // screen to reduce the performance impact
    CurrentSizeX = FMath::Clamp( int(CurrentSizeX / 1.7), 128, 1920); //1920 / 1.5 = 1280
    CurrentSizeY = FMath::Clamp( int(CurrentSizeY / 1.7), 128, 1080);

    if( CurrentSizeX == PreviousScreenSizeX
    &&  CurrentSizeY == PreviousScreenSizeY )
    {
        return;
    }

    PreviousScreenSizeX = CurrentSizeX;
    PreviousScreenSizeY = CurrentSizeY;


    // Create the RenderTarget if it does not exist
    if( PortalTexture == nullptr )
    {
        // Create new RTT
        PortalTexture = NewObject<UTextureRenderTarget2D>(
            this,
            UTextureRenderTarget2D::StaticClass(),
            *FString("PortalRenderTarget")
        );
        check( PortalTexture );

        PortalTexture->RenderTargetFormat   = ETextureRenderTargetFormat::RTF_RGBA16f;
        PortalTexture->Filter               = TextureFilter::TF_Bilinear;
        PortalTexture->SizeX                = CurrentSizeX;
        PortalTexture->SizeY                = CurrentSizeY;
        PortalTexture->ClearColor           = FLinearColor::Black;
        PortalTexture->TargetGamma          = 2.2f;
        PortalTexture->bNeedsTwoCopies      = false;
        PortalTexture->AddressX             = TextureAddress::TA_Clamp;
        PortalTexture->AddressY             = TextureAddress::TA_Clamp;

        // Not needed since the texture is displayed on screen directly
        // in some engine versions this can even lead to crashes (notably 4.24/4.25)
        PortalTexture->bAutoGenerateMips    = false;

        // This force the engine to create the render target 
        // with the parameters we defined just above
        PortalTexture->UpdateResource();
    }
    // Resize the RenderTarget if it already exists
    else
    {
        PortalTexture-> ResizeTarget( CurrentSizeX, CurrentSizeY );
    }
}
void APortalManager::Init()
{
	SceneCapture = NewObject<USceneCaptureComponent2D>(this, USceneCaptureComponent2D::StaticClass(), *FString("PortalSceneCapture"));

	SceneCapture->AttachToComponent( GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale );
	SceneCapture->RegisterComponent();

	SceneCapture->bCaptureEveryFrame            = false;
	SceneCapture->bCaptureOnMovement            = false;
	SceneCapture->LODDistanceFactor             = 3; //Force bigger LODs for faster computations
	SceneCapture->TextureTarget                 = nullptr;
	SceneCapture->bEnableClipPlane              = true;
	SceneCapture->bUseCustomProjectionMatrix    = true;
	SceneCapture->CaptureSource                 = ESceneCaptureSource::SCS_SceneColorHDRNoAlpha;

	//Setup Post-Process of SceneCapture (optimization : disable Motion Blur, etc)
	FPostProcessSettings CaptureSettings;

	CaptureSettings.bOverride_AmbientOcclusionQuality       = true;
	CaptureSettings.bOverride_MotionBlurAmount              = true;
	CaptureSettings.bOverride_SceneFringeIntensity          = true;
	CaptureSettings.bOverride_FilmGrainIntensity               = true;
	CaptureSettings.bOverride_ScreenSpaceReflectionQuality  = true;

	CaptureSettings.AmbientOcclusionQuality         = 0.0f; //0=lowest quality..100=maximum quality
	CaptureSettings.MotionBlurAmount                = 0.0f; //0 = disabled
	CaptureSettings.SceneFringeIntensity            = 0.0f; //0 = disabled
	CaptureSettings.FilmGrainIntensity                  = 0.0f; //0 = disabled
	CaptureSettings.ScreenSpaceReflectionQuality    = 0.0f; //0 = disabled
	
	CaptureSettings.bOverride_ScreenPercentage_DEPRECATED      = true;
	CaptureSettings.bOverride_ScreenPercentage_DEPRECATED      = 100.0f;

	
	SceneCapture->PostProcessSettings = CaptureSettings;
	GeneratePortalTexture();
}

void APortalManager::Update(float DeltaTime)
{ 
UpdateDelay += DeltaTime;
 
     if( UpdateDelay > 1.0f )
     {
         UpdateDelay = 0.0f;
         GeneratePortalTexture();
     }
 
     //-----------------------------------
     // Find portals in the level and update them
     //-----------------------------------
     APortals* Portal = UpdatePortalsInWorld();
 
     if( Portal != nullptr )
     {
         UpdateCapture( Portal );
     }
}

APortals* APortalManager::UpdatePortalsInWorld()
{if( ControllerOwner == nullptr )
     {
         return nullptr;
     }
 
     //-----------------------------------
     // Update Portal actors in the world (and active one if nearby)
     //-----------------------------------
     APortals* ActivePortal = nullptr;
     FVector PlayerLocation      = ControllerOwner->GetPawn()->GetActorLocation();
     float Distance              = 4096.0f;
 
     for( TActorIterator<APortals>ActorItr( GetWorld() ); ActorItr; ++ActorItr )
     {
         APortals* Portal   = *ActorItr;
         FVector PortalLocation  = Portal->GetActorLocation();
         FVector PortalNormal    = -1 * Portal->GetActorForwardVector();
 
         // Reset Portal
         Portal->ClearRTT();
         Portal->SetActive( false );
 
         // Find the closest Portal when the player is Standing in front of
         float NewDistance = FMath::Abs( FVector::Dist( PlayerLocation, PortalLocation ) );
 
         if( NewDistance < Distance )
         {
             Distance        = NewDistance;
             ActivePortal    = Portal;
         }
     }
 
     return ActivePortal;
}

void APortalManager::UpdateCapture(APortals* portal)
{ if( ControllerOwner == nullptr )
     {
         return;
     }
 
     //-----------------------------------
     // Update SceneCapture (discard if there is no active portal)
     //-----------------------------------
     if(SceneCapture     != nullptr
     && PortalTexture    != nullptr
     && portal   != nullptr
     && Character        != nullptr )
     {
 
         UCameraComponent* PlayerCamera = Character->FindComponentByClass<UCameraComponent>();
         AActor* Target  = portal->GetTarget();
 
         //Place the SceneCapture to the Target
         if( Target != nullptr )
         {
             //-------------------------------
             // Compute new location in the space of the target actor
             // (which may not be aligned to world)
             //-------------------------------
             FVector NewLocation     = APortals::ConvertLocationToActorSpace(   PlayerCamera->GetComponentLocation(),
                                                                             portal,
                                                                             Target );
 
             SceneCapture->SetWorldLocation( NewLocation );
 
             //-------------------------------
             //Compute new Rotation in the space of the
             //Target location
             //-------------------------------
             FTransform CameraTransform  = PlayerCamera->GetComponentTransform();
             FTransform SourceTransform  = portal->GetActorTransform();
             FTransform TargetTransform  = Target->GetActorTransform();
 
             FQuat LocalQuat             = SourceTransform.GetRotation().Inverse() * CameraTransform.GetRotation();
             FQuat NewWorldQuat          = TargetTransform.GetRotation() * LocalQuat;
 
             //Update SceneCapture rotation
             SceneCapture->SetWorldRotation( NewWorldQuat );
 
             //-------------------------------
             //Clip Plane : to ignore objects between the
             //SceneCapture and the Target of the portal
             //-------------------------------
             SceneCapture->ClipPlaneNormal   = Target->GetActorForwardVector();
             SceneCapture->ClipPlaneBase     = Target->GetActorLocation()
                                             + (SceneCapture->ClipPlaneNormal * -1.5f); //Offset to avoid visible pixel border
         }
 
         // Switch on the valid Portal
         portal->SetActive( true );
 
         // Assign the Render Target
         portal->SetRTT( PortalTexture );
         SceneCapture->TextureTarget = PortalTexture;
 
         // Get the Projection Matrix
         
         // Say Cheeeeese !
         SceneCapture->CaptureScene();
     }
}

// Called when the game starts or when spawned
void APortalManager::BeginPlay()
{
	Super::BeginPlay();

}



FMatrix APortalManager::GetCameraProjectionMatrix()
{
	FMatrix ProjectionMatrix;
 
     if( GetWorld()->GetFirstLocalPlayerFromController() != nullptr )
     {
         FSceneViewProjectionData PlayerProjectionData;
 
         GetWorld()->GetFirstLocalPlayerFromController()->GetProjectionData( GetWorld()->GetFirstLocalPlayerFromController()->ViewportClient->Viewport,PlayerProjectionData );
 
         ProjectionMatrix = PlayerProjectionData.ProjectionMatrix;
     }
 
     return ProjectionMatrix;
}



// Called every frame
void APortalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

