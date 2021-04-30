#include "FGVisionSensingComponent.h"

#include "CollisionDebugDrawingPublic.h"
#include "FGVisionSensingSettings.h"
#include "FGVisionSensingTargetComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

UFGVisionSensingComponent::UFGVisionSensingComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UFGVisionSensingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (SensingSettings == nullptr)
		return;

	const FVector Direction = GetOwner()->GetActorForwardVector();
	const FVector Origin = GetOwner()->GetActorLocation();

	if (bDebugDrawVision)
	{
		FVector Right = Direction.RotateAngleAxis(SensingSettings->Angle, FVector::UpVector);
		FVector Left = Direction.RotateAngleAxis(-SensingSettings->Angle, FVector::UpVector);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Right * SensingSettings->Distance, FLinearColor::Red);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Left * SensingSettings->Distance, FLinearColor::Green);
	}

	
	for (int32 Index = SensedTargets.Num() - 1; Index >= 0; --Index)
	{
		UFGVisionSensingTargetComponent* Target = SensedTargets[Index];
		
		if (Target == nullptr || (Target != nullptr && Target->IsBeingDestroyed()))
		{
			SensedTargets.RemoveAt(Index);
			continue;
		}

		if (!IsActorVisible(Target->GetOwner(), Origin, Direction, SensingSettings->Distance))
		{
			FFGVisionSensingResults Results;
			Results.SensedActor = Target->GetOwner();
			OnTargetLost.Broadcast(Results);
			SensedTargets.RemoveAt(Index);
		}
	}

	TArray<UFGVisionSensingTargetComponent*> ListOfTargets;
	
	UFGVisionSensingTargetComponent::GetSensingTargets(ListOfTargets, GetOwner()->GetActorLocation(), SensingSettings->Distance);

	
	for (UFGVisionSensingTargetComponent* Target : ListOfTargets)
	{
		AActor* TargetActor = Target->GetOwner();
		
		if (IsActorVisible(TargetActor, Origin, Direction, SensingSettings->Distance))
		{
			FFGVisionSensingResults Results;
			Results.SensedActor = TargetActor;
			if (!SensedTargets.Contains(Target))
			{
				SensedTargets.Add(Target);				
				OnTargetSensed.Broadcast(Results);				
			}

			const FVector DirectionToTarget = (TargetActor->GetActorLocation() - Origin).GetSafeNormal();
			float DistanceSq = FMath::Square(SensingSettings->DistanceInner);
			
			const auto InnerAngleThreshold = FMath::Cos(FMath::DegreesToRadians(SensingSettings->InnerAngle));	
			const float Dot = FVector::DotProduct(Direction, DirectionToTarget);
			if (Dot > InnerAngleThreshold
				&& FVector::DistSquared(Origin, TargetActor->GetActorLocation()) < DistanceSq)
			{
				OnTargetInCentralVision.Broadcast(Results);
			}			
		}		
	}
}

bool UFGVisionSensingComponent::IsActorVisible(const AActor* ActorToTest, const FVector& Origin,
	const FVector& Direction, float DistanceMinimum) const
{
	if (SensingSettings == nullptr)
		return false;

	float DistanceMinimumSq = FMath::Square(DistanceMinimum);

	if (FVector::DistSquared(Origin, ActorToTest->GetActorLocation()) > DistanceMinimumSq)
	{
		return false;
	}

	const FVector DirectionToTarget = (ActorToTest->GetActorLocation() - Origin).GetSafeNormal();

	const float OuterAngleThreshold = FMath::Cos(FMath::DegreesToRadians(SensingSettings->Angle));
	const float Dot = FVector::DotProduct(Direction, DirectionToTarget);

	
	if (Dot > OuterAngleThreshold)
	{
		const FName TraceTag("Enemy LOS");
		if (bDebugDrawVision)
			GetWorld()->DebugDrawTraceTag = TraceTag;	

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, Origin, ActorToTest->GetActorLocation(), ECC_Visibility,
            FCollisionQueryParams(TraceTag, false, nullptr));		
		
		if (!Hit.bBlockingHit || Hit.Actor == ActorToTest)
			return true;		
	}
	
	return false;
}

float UFGVisionSensingComponent::GetVisionInRadians() const
{
	if (SensingSettings == nullptr)
		return 0.0;

	return FMath::Cos(FMath::DegreesToRadians(SensingSettings->Angle));
}

