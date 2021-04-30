
#pragma once

#include "Engine/DataAsset.h"
#include "FGNoiseSettings.generated.h"

UCLASS()
class FGAI_API UFGNoiseSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Sensing)
	float Radius = 150.0f;
	UPROPERTY(EditAnywhere, Category = Sensing)
	float LifeTime = 1.0f;
};
