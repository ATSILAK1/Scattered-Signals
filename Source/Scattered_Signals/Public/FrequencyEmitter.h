// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "InputAction.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FrequencyEmitter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FrequencyChangeDelegate, TArray<float>);
DECLARE_MULTICAST_DELEGATE_OneParam(FrequencyChannelChangeDelegate, int);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FrequencyDeviceEnabled,bool , isenabled);


class UInputAction;


UCLASS()
class SCATTERED_SIGNALS_API AFrequencyEmitter : public AActor
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	AFrequencyEmitter();

	// Frequency Device Related



	// Variable if the Actor is enabled
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	bool IsEnabled ;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	TArray<float> FrequencyToDiscover ;

	
	// ALL the frequencies Known By the player 
	UPROPERTY(EditAnywhere)
	TArray<float> FrequencySaved;
	UPROPERTY(EditAnywhere)
	int FrequencySavedArrayIndex; 
	
	
	// All the frequency thats being emitted by the device 
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	TArray<float> frequencyToEmitt; 

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	int FrequencyChannel ;
	

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* sphereCollider;

	
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	FrequencyDeviceEnabled deviceEnabled ;
	FrequencyChangeDelegate frequencyChangeDelegate;
	FrequencyChannelChangeDelegate FrequencyChannelChangeDelegate;

	
	// Input related IMC And IA 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FrequencyEmitterMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* IncreaseFreaquencyAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* DeacreaseFreaquencyAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* DeacreaseChannelAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* IncreaseChannelAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* GoToTheNextKnownFrequencyValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* GoToThePreviousKnownFrequencyValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* Interact;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	
	// Called every frame
	virtual void Tick(float DeltaTime) override;




	void EmittFrequency();
	UFUNCTION(BlueprintCallable)
	void IsFrequencyEmitterAvailable();
	void IncreaseFrequency(float);
	void DecreaseFrequency(float);
	void FrequencyChannelChange(float);
	float Round2Decimal(float Number);
	UFUNCTION(BlueprintCallable)
	bool ContainAFrequency(float Number , float Frequency);

	// Frequency Managment :
		// increase and decrease Frequency
	void AddFrequencyFunctionOnStarted(const FInputActionValue& Value);
	void DecreaseFrequencyFunctionOnStarted(const FInputActionValue& Value);
		// Increase and decrease the channel 
	void DecreaseChannelFunction(const FInputActionValue& Value);
	void IncreaseChannelFunction(const FInputActionValue& Value);
		// Go directly to known frequencies 
	void NextKnownFrequencyValue(const FInputActionValue& Value);
	void PreviousKnownFrequencyValue( const FInputActionValue& Value);
	
	void InteractWithInteractableObject(const FInputActionValue& Value);	
	// Door Switching 
	void SwitchDoorState();

	// Vibration 
	void ControllerVibration();
	
	UFUNCTION()
	void OnOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
