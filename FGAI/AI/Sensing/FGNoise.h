// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FGNoiseSettings.h"
#include "GameFramework/Actor.h"
#include "FGNoise.generated.h"



UCLASS()
class FGAI_API AFGNoise : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* NoiseCause = nullptr;
	
	UPROPERTY()	
	UFGNoiseSettings* NoiseSettings;
	
public:
	UPROPERTY(BlueprintReadOnly)
	USceneComponent* SceneComponent;

	float Radius = 100.f;
	float LifeTime = 0.f;
	
	// Sets default values for this actor's properties
	AFGNoise();

	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	static void SpawnNoiseActor(FVector Location, AActor* NoiseInstigator, UFGNoiseSettings* Settings);
};

