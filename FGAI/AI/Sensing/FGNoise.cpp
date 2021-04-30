// Fill out your copyright notice in the Description page of Project Settings.


#include "FGNoise.h"

#include "FGAI/FGAIGameInstance.h"


// Sets default values
AFGNoise::AFGNoise()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene")); 
	RootComponent = SceneComponent;
}

// Called when the game starts or when spawned
void AFGNoise::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AFGNoise::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	LifeTime = LifeTime - DeltaTime;
	if (LifeTime <= 0.f)
	{
		Cast<UFGAIGameInstance>(GetGameInstance())->ActiveNoises.Remove(this);
		Destroy();
	}
}

void AFGNoise::SpawnNoiseActor(FVector Location, AActor* NoiseInstigator, UFGNoiseSettings* Settings)
{
	UWorld* World = NoiseInstigator->GetWorld();
	AFGNoise* Noise = World->SpawnActor<AFGNoise>(Location, FRotator());
	Noise->NoiseSettings = Settings;
	Noise->Radius = Settings->Radius;
	Noise->LifeTime = Settings->LifeTime;
	
	UFGAIGameInstance* Instance = Cast<UFGAIGameInstance>(World->GetGameInstance());	
	Instance->ActiveNoises.Add(Noise);	
}

