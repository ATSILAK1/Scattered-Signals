//// Fill out your copyright notice in the Description page of Project Settings.


// Fill out your copyright notice in the Description page of Project Settings.

#include "FrequencyEmitter.h"

#include "DoorManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FrequencyResponder.h"
#include "Interactable.h"
#include "Components/SphereComponent.h"

// Sets default values
AFrequencyEmitter::AFrequencyEmitter()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

   
    sphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
    RootComponent = sphereCollider; 

   
    sphereCollider->InitSphereRadius(300.0f); 
    sphereCollider->SetCollisionProfileName(TEXT("OverlapAllDynamic")); 

    
    sphereCollider->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AFrequencyEmitter::BeginPlay()
{
    Super::BeginPlay();
   
    for (int i = 0; i < frequencyToEmitt.Num(); ++i)
    {
        UE_LOG(LogTemp,Display,TEXT("Frequency %f"),frequencyToEmitt[i]);
    }

    if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        // Get the enhanced input subsystem
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            // Add the mapping context
            Subsystem->AddMappingContext(FrequencyEmitterMappingContext, 0);
        }


        if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
        {
            // Fire
            EnhancedInputComponent->BindAction(IncreaseChannelAction, ETriggerEvent::Started, this, &AFrequencyEmitter::IncreaseChannelFunction);
            EnhancedInputComponent->BindAction(DeacreaseChannelAction, ETriggerEvent::Started, this, &AFrequencyEmitter::DecreaseChannelFunction);
            EnhancedInputComponent->BindAction(IncreaseFreaquencyAction, ETriggerEvent::Triggered, this, &AFrequencyEmitter::AddFrequencyFunctionOnStarted);
            EnhancedInputComponent->BindAction(DeacreaseFreaquencyAction, ETriggerEvent::Triggered, this, &AFrequencyEmitter::DecreaseFrequencyFunctionOnStarted);
            EnhancedInputComponent->BindAction(GoToTheNextKnownFrequencyValue , ETriggerEvent::Started ,this , &AFrequencyEmitter::NextKnownFrequencyValue);
            EnhancedInputComponent->BindAction(GoToThePreviousKnownFrequencyValue , ETriggerEvent::Started ,this , &AFrequencyEmitter::PreviousKnownFrequencyValue);
            EnhancedInputComponent->BindAction(Interact,ETriggerEvent::Started,this,&AFrequencyEmitter::SwitchDoorState);
            EnhancedInputComponent->BindAction(Interact,ETriggerEvent::Completed, this , &AFrequencyEmitter::InteractWithInteractableObject);

        }
    }
    // Bind overlap event
    if (sphereCollider)
    {
        sphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AFrequencyEmitter::OnOverLapBegin);
    }

    frequencyChangeDelegate.Broadcast(frequencyToEmitt);
}

void AFrequencyEmitter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    IsFrequencyEmitterAvailable();
    
}

void AFrequencyEmitter::OnOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this || !OtherComp)
    {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Overlap Begin with: %s (Component: %s)"),
        *OtherActor->GetName(),
        *OtherComp->GetName());

    //for ( int i = 0 ; i < frequencyToEmitt.Num(); i++)
    //    frequencyChangeDelegate.Broadcast(frequencyToEmitt[i]);
}


void AFrequencyEmitter::EmittFrequency()
{
        if (frequencyChangeDelegate.IsBound() )
            frequencyChangeDelegate.Broadcast(frequencyToEmitt );
        else
            UE_LOG(LogTemp, Warning, TEXT("is not bound "));
}
void AFrequencyEmitter::IsFrequencyEmitterAvailable()
{
    if (!IsEnabled)
    {
        TArray<float> Temps = {0,0,0};
        frequencyChangeDelegate.Broadcast(Temps);
        
    }
   
        
}

void AFrequencyEmitter::IncreaseFrequency(float ValueAdded)
{
    frequencyToEmitt[FrequencyChannel] += ( ValueAdded );
    frequencyToEmitt[FrequencyChannel] = Round2Decimal(frequencyToEmitt[FrequencyChannel]);
}

void AFrequencyEmitter::DecreaseFrequency(float ValueMinus)
{
    frequencyToEmitt[FrequencyChannel] -= ValueMinus;

    frequencyToEmitt[FrequencyChannel] = Round2Decimal(frequencyToEmitt[FrequencyChannel]);
   
    if (frequencyToEmitt[FrequencyChannel] <= 0)
        frequencyToEmitt[FrequencyChannel] = 0 ; 
}

void AFrequencyEmitter::FrequencyChannelChange(float input)
{
    FrequencyChannel += input;

    if (FrequencyChannel >= frequencyToEmitt.Num()) FrequencyChannel = frequencyToEmitt.Num()-1;
    if (FrequencyChannel < 0) FrequencyChannel = 0;
    
    FrequencyChannelChangeDelegate.Broadcast(FrequencyChannel);
}

float AFrequencyEmitter::Round2Decimal(float Number)
{
    return FMath::RoundToFloat(Number * 100.0f)/ 100.0f;
}

bool AFrequencyEmitter::ContainAFrequency(float Number , float Frequency )
{
    if (frequencyToEmitt.Contains(Number)  &&  FMath::IsNearlyEqual(Number , Frequency , 0.1f))
    {
        return true;
    }
   return false ;
}

void AFrequencyEmitter::AddFrequencyFunctionOnStarted(const FInputActionValue& Value)
{
    IncreaseFrequency(0.8 * GetWorld()->GetDeltaSeconds());
    
    
    frequencyChangeDelegate.Broadcast(frequencyToEmitt );
}

void AFrequencyEmitter::DecreaseFrequencyFunctionOnStarted(const FInputActionValue& Value)
{
    DecreaseFrequency(0.8 * GetWorld()->GetDeltaSeconds());
 
    frequencyChangeDelegate.Broadcast(frequencyToEmitt );
}



void AFrequencyEmitter::DecreaseChannelFunction(const FInputActionValue& Value)
{
    FrequencyChannelChange(-1);
    FrequencyChannelChangeDelegate.Broadcast(FrequencyChannel);
}

void AFrequencyEmitter::IncreaseChannelFunction(const FInputActionValue& Value)
{
    
    FrequencyChannelChange(1);
    FrequencyChannelChangeDelegate.Broadcast(FrequencyChannel);
}

void AFrequencyEmitter::NextKnownFrequencyValue(const FInputActionValue& Value)
{
    FrequencySavedArrayIndex += 1 % FrequencySaved.Num();
    frequencyToEmitt[FrequencyChannel] = FrequencySaved[FrequencySavedArrayIndex];
    frequencyChangeDelegate.Broadcast(frequencyToEmitt );
    UE_LOG(LogTemp, Warning, TEXT("FrequencySavedArrayIndex:++++ %d"), FrequencySavedArrayIndex);
}

void AFrequencyEmitter::PreviousKnownFrequencyValue(const FInputActionValue&)
{
    FrequencySavedArrayIndex -= 1 % FrequencySaved.Num();
    frequencyToEmitt[FrequencyChannel] = FrequencySaved[FrequencySavedArrayIndex];
    frequencyChangeDelegate.Broadcast(frequencyToEmitt );
    UE_LOG(LogTemp, Warning, TEXT("FrequencySavedArrayIndex:---- %d"), FrequencySavedArrayIndex);
}

void AFrequencyEmitter::InteractWithInteractableObject(const FInputActionValue& Value)
{
    APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn(); 

    if (Player == nullptr)
        UE_LOG(LogTemp, Warning, TEXT("The Player Here is null"));

    FVector StartLocation = Player->GetActorLocation();
    FVector EndLocation = StartLocation + Player->GetActorForwardVector() * 100;
    FHitResult Hit;
    FCollisionQueryParams Parameters(FName(TEXT("HitResult")),false,GetOwner());
    Parameters.AddIgnoredActor(this);
    Parameters.AddIgnoredActor(Player);
    GetWorld()->LineTraceSingleByObjectType(Hit,StartLocation,EndLocation,FCollisionObjectQueryParams::AllObjects,Parameters);

    //DrawDebugDirectionalArrow(
    //GetWorld(),
    //StartLocation,
    //EndLocation ,
    //120.0f, // Arrow size
    //FColor::Red,
    //true, // Persistent
    //10,
    //0, // Depth priority
    //2.0f // Thickness
//);

    if (Hit.GetActor() == nullptr) // check if the actor is null 
    {
        UE_LOG(LogTemp, Warning, TEXT("The Object here in Frequency Emiter is null"));
        return ;
    }
  
    
    if ( Hit.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()) ) // check if the actor hit by the raycast implement the interface 
    {
             IInteractable* Interaface =  Cast<IInteractable>(Hit.GetActor());
                Interaface->Interact();
            
          
    }
    else
    {
        UE_LOG(LogTemp,Warning,TEXT("Dont implement interface"));
    }
    
   
}

void AFrequencyEmitter::SwitchDoorState()
{
    for (int i = 0 ; i < frequencyToEmitt.Num() ; i++)
    {
        if (FMath::IsNearlyEqual( 0.5,frequencyToEmitt[i],0.1))
        ADoorManager::SwitchDoorStateDelegate.Broadcast(); // Broadcast a switch door event to go form open <=> close 
        
    }
}

void AFrequencyEmitter::ControllerVibration()
{
    
}



