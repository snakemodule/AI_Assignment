// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FGNoise.h"
#include "Components/ActorComponent.h"
#include "FGHearingSenseComponent.generated.h"


USTRUCT(BlueprintType)
struct FFGHearingSensingResults
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	AFGNoise* HeardNoise = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFGHearingSensingDelegate, const FFGHearingSensingResults&, Results);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FGAI_API UFGHearingSenseComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TSet<AFGNoise*> HeardNoises; 
	
public:

	UPROPERTY(BlueprintAssignable)
	FFGHearingSensingDelegate OnNoiseHearing;

	UPROPERTY(BlueprintAssignable)
	FFGHearingSensingDelegate OnNoiseLost;

	
	//UPROPERTY(EditAnywhere)
	//float HearingRange = 1000.f;
	
	// Sets default values for this component's properties
	UFGHearingSenseComponent();
	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
