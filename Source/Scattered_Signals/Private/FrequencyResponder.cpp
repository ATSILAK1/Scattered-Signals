// Fill out your copyright notice in the Description page of Project Settings.


#include "FrequencyResponder.h"

#include <string>

#include "FrequencyEmitter.h"
#include "EngineUtils.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UFrequencyResponder::UFrequencyResponder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFrequencyResponder::BeginPlay()
{
	
	Super::BeginPlay();
	AFrequencyEmitter* FrequencyEmitter= nullptr ;

	for (TActorIterator<AFrequencyEmitter> It(GetWorld()); It; ++It)
	{
		FrequencyEmitter = *It;
		if (FrequencyEmitter)
		{
			
			UE_LOG(LogTemp, Warning, TEXT("Found FrequencyEmitter: %s"), *FrequencyEmitter->GetName());
			break; 
		}
	}

	if (FrequencyEmitter)
	{
		FrequencyEmitter->frequencyChangeDelegate.AddUObject(this, &UFrequencyResponder::FrequencyChangedCallBack);
		
		UE_LOG(LogTemp, Warning, TEXT("Successfully subscribed to delegate"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find FrequencyEmitter in level"));
	}
}


// Called every frame
void UFrequencyResponder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



// Function thats hide and show object in the game 
void UFrequencyResponder::FrequencyChangedCallBack(TArray<float> frequency )
{

	
	AActor* actor = GetOwner();
	if (actor == nullptr)
		return;

	

	for (int i = 0; i < frequency.Num(); i++)
	{
		if (FMath::IsNearlyEqual( frequency[i] , this->FrequencyResponse , 0.1f) )
		{
			actor->SetActorHiddenInGame(false);
			
			
			UE_LOG(LogTemp,Warning,TEXT("the actor is hidden "))
			break;
		
		}
		else
			actor->SetActorHiddenInGame(true);

		//GEngine->AddOnScreenDebugMessage(i, 5, FColor::Red,FString::Printf( TEXT(" Channel %d : %.2f"), i , frequency[i]) , true ,FVector2D::One() * 2);
	}
}


