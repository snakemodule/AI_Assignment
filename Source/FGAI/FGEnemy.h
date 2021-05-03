#pragma once

#include "GameFramework/Pawn.h"
#include "FGEnemy.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UCameraComponent;
class UFGVisionSensingComponent;
class UFGNavMovementComponent;

UCLASS()
class AFGEnemy : public APawn
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleDefaultsOnly, Category=Collision)
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Vision)
	UFGVisionSensingComponent* VisionSensingComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	UFGNavMovementComponent* NavMovementComponent;

	AFGEnemy();

	virtual float GetDefaultHalfHeight() const override;

protected:
	virtual void BeginPlay();

	UCapsuleComponent* GetCapsule() const { return Capsule; }
};

