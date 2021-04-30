// Fill out your copyright notice in the Description page of Project Settings.


#include "FGHearingSenseComponent.h"

#include "FGNoise.h"
#include "FGAI/FGAIGameInstance.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UFGHearingSenseComponent::UFGHearingSenseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFGHearingSenseComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFGHearingSenseComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//TArray<AActor*> Noises;	
	//UGameplayStatics::GetAllActorsOfClass(this, AFGNoise::StaticClass(), Noises);
	//set in game instance
	auto& ActiveNoises = Cast<UFGAIGameInstance>(GetOwner()->GetGameInstance())->ActiveNoises;	

	//If a noise I am hearing does not exist anymore -> broadcast
	for (auto Noise : HeardNoises)
	{
		if (!ActiveNoises.Contains(Noise))
		{
			OnNoiseLost.Broadcast({Noise});
		}		
	}
	HeardNoises = HeardNoises.Intersect(ActiveNoises);
	
	for (auto Noise : ActiveNoises)
	{
		auto NoiseLocation = Noise->GetActorLocation();
		auto ListenerLocation = GetOwner()->GetActorLocation();
		auto DistSqr = FVector::DistSquared(NoiseLocation, ListenerLocation);
		if (DistSqr <= Noise->Radius*Noise->Radius && !HeardNoises.Contains(Noise))
		{
			HeardNoises.Add(Noise);
			OnNoiseHearing.Broadcast({Noise});
		}		
	}
	
	
}

