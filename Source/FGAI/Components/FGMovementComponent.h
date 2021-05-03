#pragma once

#include "GameFramework/MovementComponent.h"
#include "FGMovementComponent.generated.h"

struct FFGFrameMovement;

UCLASS()
class UFGMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FFGFrameMovement CreateFrameMovement() const;

	void Move(FFGFrameMovement& FrameMovement);

	FRotator GetFacingRotation() const { return FacingRotationCurrent; }
	FVector GetFacingDirection() const { return FacingRotationCurrent.Vector(); }

	void SetFacingRotation(const FRotator& InFacingRotation, float InRotationSpeed = -1.0f);
	void SetFacingRotation(const FQuat& InFacingRotation, float InRotationSpeed = -1.0f);
	void SetFacingDirection(const FVector& InFacingDirection, float InRotationSpeed = -1.0f);

private:
	void Internal_SetFacingRotation(const FRotator& InFacingRotation, float InRotationSpeed);
	FVector GetMovementDelta(const FFGFrameMovement& FrameMovement) const;

	FRotator FacingRotationCurrent;
	FRotator FacingRotationTarget;
	float FacingRotationSpeed;
};