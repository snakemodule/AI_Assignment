// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AI/Sensing/FGNoise.h"

#include "FGAIGameInstance.generated.h"

/**
 * 
 */
UCLASS(config=Game, transient, BlueprintType, Blueprintable)
class FGAI_API UFGAIGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()
public:
	//UFGAIGameInstance(const FObjectInitializer& ObjectInitializer);
	
	TSet<AFGNoise*> ActiveNoises;	
};
