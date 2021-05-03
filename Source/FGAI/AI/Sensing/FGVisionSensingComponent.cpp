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
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Right * SensingSettings->DistanceMinimum, FLinearColor::Red);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Left * SensingSettings->DistanceMinimum, FLinearColor::Green);
	}

	for (int32 Index = SensedTargets.Num() - 1; Index >= 0; --Index)
	{
		UFGVisionSensingTargetComponent* Target = SensedTargets[Index];

		if (Target == nullptr || (Target != nullptr && Target->IsBeingDestroyed()))
		{
			SensedTargets.RemoveAt(Index);
			continue;
		}

		if (!IsActorVisible(Target->GetOwner(), Origin, Direction, SensingSettings->DistanceMinimum))
		{
			FFGVisionSensingResults Results;
			Results.SensedActor = Target->GetOwner();
			OnTargetLost.Broadcast(Results);
			SensedTargets.RemoveAt(Index);
		}
	}

	TArray<UFGVisionSensingTargetComponent*> ListOfTargets;
	
	UFGVisionSensingTargetComponent::GetSensingTargets(ListOfTargets, GetOwner()->GetActorLocation(), SensingSettings->DistanceMinimum);

	for (UFGVisionSensingTargetComponent* Target : ListOfTargets)
	{
		
		
		//UKismetSystemLibrary::LineTraceSingle()
		//ETraceTypeQuery
		//UKismetSystemLibrary::LineTraceSingle(this, Origin, Target->GetTargetOrigin(), , false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, Hit, true);		
		//if (Hit.bBlockingHit)
		//{
		//	
		//}
		//GetWorld()->LineTraceSingleByObjectType(Hit, Origin, Origin+DirectionToTarget*10000,
    	//FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllObjects),
    	//FCollisionQueryParams(TEXT("Enemy LOS"), false, nullptr));
		//Target->GetOwner()->ActorLineTraceSingle(Hit, Origin, Origin+DirectionToTarget*10000, ECC_Camera, FCollisionQueryParams(TraceTag, false));

		
		
		if (!SensedTargets.Contains(Target) &&
			IsActorVisible(Target->GetOwner(), Origin, Direction, SensingSettings->DistanceMinimum))
		{
			SensedTargets.Add(Target);
			FFGVisionSensingResults Results;
			Results.SensedActor = Target->GetOwner();
			OnTargetSensed.Broadcast(Results);
		}
	}
}



bool UFGVisionSensingComponent::IsPointVisible(const FVector& PointToTest, const FVector& Origin, const FVector& Direction, float DistanceMinimum) const
{
	if (SensingSettings == nullptr)
		return false;

	float DistanceMinimumSq = FMath::Square(DistanceMinimum);
	

	if (FVector::DistSquared(Origin, PointToTest) > DistanceMinimumSq)
	{
		return false;
	}

	const FVector DirectionToTarget = (PointToTest - Origin).GetSafeNormal();

	const float AsHalfRad = FMath::Cos(FMath::DegreesToRadians(SensingSettings->Angle));
	const float Dot = FVector::DotProduct(Direction, DirectionToTarget);

	const bool bIsValid = Dot > AsHalfRad;

	return bIsValid;
}

bool UFGVisionSensingComponent::IsActorVisible(const AActor* ActorToTest, const FVector& Origin,
	const FVector& Direction, float DistanceMinimum) const
{
	if (SensingSettings == nullptr)
		return false;

	float DistanceMinimumSq = FMath::Square(DistanceMinimum);

		
	//GetWorld()->LineTraceSingleByChannel()

	if (FVector::DistSquared(Origin, ActorToTest->GetActorLocation()) > DistanceMinimumSq)
	{
		return false;
	}

	const FVector DirectionToTarget = (ActorToTest->GetActorLocation() - Origin).GetSafeNormal();

	const float AsHalfRad = FMath::Cos(FMath::DegreesToRadians(SensingSettings->Angle));
	const float Dot = FVector::DotProduct(Direction, DirectionToTarget);

	if (Dot > AsHalfRad)
	{
		const FName TraceTag("Enemy LOS"); 
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

