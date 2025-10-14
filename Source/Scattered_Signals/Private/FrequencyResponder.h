// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FrequencyResponder.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UFrequencyResponder : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFrequencyResponder();

	UPROPERTY(EditAnywhere)
	float FrequencyResponse;

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
	void FrequencyChangedCallBack(TArray<float> frequency );
	


		
};
