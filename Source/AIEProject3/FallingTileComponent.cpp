// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingTileComponent.h"
#include "GameFramework/Character.h"
#include "Engine/HitResult.h"

// Sets default values for this component's properties
UFallingTileComponent::UFallingTileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DrawDebugBox();
}

UFallingTileComponent::~UFallingTileComponent()
{
	if(GetWorld())
	{
		FlushPersistentDebugLines(GetWorld());
	}
}


// Called when the game starts
void UFallingTileComponent::BeginPlay()
{
	Super::BeginPlay();

	//Store reference to the owning actor
	OwningActor = GetOwner();

	//Break if nullptr
	if(!OwningActor) return;

	//Checks that the owning actor has physics disabled
	if(OwningActor->GetRootComponent()->IsSimulatingPhysics())
	{
		//tell the designer they are silly and need to enable physics 
		UE_LOG(LogTemp, Warning, TEXT("Object %s does not require physics. Consider disabling for better performance."), *OwningActor->GetName());
		return;
	}

	UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(OwningActor->GetRootComponent());
	if (!Primitive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Casting %s to Primitive failed"), *OwningActor->GetName());
	}

	//Disables gravuty
	Primitive->SetEnableGravity(false);

	//checks constraints
	if(!Primitive->BodyInstance.bLockXTranslation)
	{
		UE_LOG(LogTemp, Warning, TEXT("Object %s requires all constraints to be locked to be enabled for FallingTile to run."), *OwningActor->GetName());
		return;
	}

}


// Called every frame
void UFallingTileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Break if actor is null
	if (!OwningActor) return;

	if(!bPlayerTouched)
	{
		switch (ChosenCollisionDetection)
		{
		case ECollisionDetectionMode::SphereTrigger:
			DoSphereTrace();
			break;

		case ECollisionDetectionMode::BoxTrigger:
			DoBoxTrace();
			break;
		}
	}

	if (bShouldFall)
	{
		Fall(DeltaTime);
	}
}

//void UFallingTileComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
//{
//	Super::PostEditChangeProperty(PropertyChangedEvent);
//	
//	DrawDebugBox();
//}

void UFallingTileComponent::DoSphereTrace()
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner()); //ignores the owning actor from the trace

	//get position
	FVector Start = GetOwner()->GetActorLocation();
	Start.Z += CollisionCenterOffset;

	//do the sphere trace
	FHitResult HitResult;
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		Start,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(CollisionSphereRadius),
		QueryParams
	);

	//draw the sphere if in debug mode
	if (bDebugMode)
	{
		DrawDebugSphere(GetWorld(), Start, CollisionSphereRadius, 12, FColor::Red);
	}

	//if the sphere cast touched something
	if (bHit)
	{
		//try to cast to player
		if (ACharacter* MyCharacter = Cast<ACharacter>(HitResult.GetActor()))
		{
			if (WaitTime == 0)
			{
				BeginFall();
			}
			else
			{
				GetWorld()->GetTimerManager().SetTimer(FallTimerHandle, this, &UFallingTileComponent::BeginFall, WaitTime);
			}

			bPlayerTouched = true;
		}

	}

	
}

void UFallingTileComponent::DrawDebugBox()
{
	//gets rid of the old lines
	FlushPersistentDebugLines(GetWorld());

	//check we want to see those stupid ass lines
	if (bDebugMode)
	{
		//check we are actually in the world
		//or unreal engine will fucking crash before you even load
		//stupid fucking ass engine bitch
		if(GetOwner() && GetWorld())
		{
			//centre of the box
			FVector Start = GetOwner()->GetActorLocation();
			Start.Z += CollisionCenterOffset;

			//have two corners to reuse because we love saving memory yippee
			FVector Corner1;
			FVector Corner2;
			
			//draw bottom of box
			Corner1 = {HalfDimensions.X, HalfDimensions.Y, -HalfDimensions.Z};
			Corner1 += Start;
			Corner2 = {-HalfDimensions.X, HalfDimensions.Y, -HalfDimensions.Z};
			Corner2 += Start;
			DrawDebugLine(GetWorld(), Corner1, Corner2, FColor::Red, true);

			Corner1 = {-HalfDimensions.X, -HalfDimensions.Y, -HalfDimensions.Z};
			Corner1 += Start;
			DrawDebugLine(GetWorld(), Corner1, Corner2, FColor::Red, true);

			Corner2 = {HalfDimensions.X, -HalfDimensions.Y, -HalfDimensions.Z};
			Corner2 += Start;
			DrawDebugLine(GetWorld(), Corner1, Corner2, FColor::Red, true);

			Corner1 = {HalfDimensions.X, HalfDimensions.Y, -HalfDimensions.Z};
			Corner1 += Start;
			DrawDebugLine(GetWorld(), Corner1, Corner2, FColor::Red, true);

			//draw top of box
			Corner1 = {HalfDimensions.X, HalfDimensions.Y, HalfDimensions.Z};
			Corner1 += Start;
			Corner2 = {-HalfDimensions.X, HalfDimensions.Y, HalfDimensions.Z};
			Corner2 += Start;
			DrawDebugLine(GetWorld(), Corner1, Corner2, FColor::Red, true);

			Corner1 = {-HalfDimensions.X, -HalfDimensions.Y, HalfDimensions.Z};
			Corner1 += Start;
			DrawDebugLine(GetWorld(), Corner1, Corner2, FColor::Red, true);

			Corner2 = {HalfDimensions.X, -HalfDimensions.Y, HalfDimensions.Z};
			Corner2 += Start;
			DrawDebugLine(GetWorld(), Corner1, Corner2, FColor::Red, true);

			Corner1 = {HalfDimensions.X, HalfDimensions.Y, HalfDimensions.Z};
			Corner1 += Start;
			DrawDebugLine(GetWorld(), Corner1, Corner2, FColor::Red, true);

			//draw side of box
			Corner1 = {HalfDimensions.X, HalfDimensions.Y, HalfDimensions.Z};
			Corner1 += Start;
			Corner2 = {HalfDimensions.X, HalfDimensions.Y, -HalfDimensions.Z};
			Corner2 += Start;
			DrawDebugLine(GetWorld(), Corner1, Corner2, FColor::Red, true);
			
			Corner1 = {-HalfDimensions.X, HalfDimensions.Y, HalfDimensions.Z};
			Corner1 += Start;
			Corner2 = {-HalfDimensions.X, HalfDimensions.Y, -HalfDimensions.Z};
			Corner2 += Start;
			DrawDebugLine(GetWorld(), Corner1, Corner2, FColor::Red, true);

			Corner1 = {-HalfDimensions.X, -HalfDimensions.Y, HalfDimensions.Z};
			Corner1 += Start;
			Corner2 = {-HalfDimensions.X, -HalfDimensions.Y, -HalfDimensions.Z};
			Corner2 += Start;
			DrawDebugLine(GetWorld(), Corner1, Corner2, FColor::Red, true);

			Corner1 = {HalfDimensions.X, -HalfDimensions.Y, HalfDimensions.Z};
			Corner1 += Start;
			Corner2 = {HalfDimensions.X, -HalfDimensions.Y, -HalfDimensions.Z};
			Corner2 += Start;
			DrawDebugLine(GetWorld(), Corner1, Corner2, FColor::Red, true);
		}
		
	}
}

void UFallingTileComponent::DoBoxTrace()
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner()); //ignores the owning actor from the trace

	//get position
	FVector Start = GetOwner()->GetActorLocation();
	Start.Z += CollisionCenterOffset;
	
	//make the box
	FCollisionShape BoxShape = FCollisionShape::MakeBox(HalfDimensions);

	//do the box trace
	FHitResult HitResult;
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		Start,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		BoxShape,
		QueryParams
	);

	//if the sphere cast touched something
	if (bHit)
	{
		//try to cast to player
		if (ACharacter* MyCharacter = Cast<ACharacter>(HitResult.GetActor()))
		{
			if (WaitTime == 0)
			{
				BeginFall();
			}
			else
			{
				GetWorld()->GetTimerManager().SetTimer(FallTimerHandle, this, &UFallingTileComponent::BeginFall, WaitTime);
			}

			bPlayerTouched = true;
		}

	}
}

void UFallingTileComponent::Fall(float DeltaTime)
{
	FVector TargetLocation = OwningActor->GetActorLocation();
	
	switch(ChosenFallMode)
	{
	case EFallMode::Acceleration:
		TargetLocation.Z -= DeltaTime * InitialFallSpeed;
		InitialFallSpeed += FallSpeedAcceleration * DeltaTime;
		break;
	case EFallMode::ConstantSpeed:
		TargetLocation.Z -= DeltaTime * FallSpeed;
		break;
	default:
		break;
	}
	
	OwningActor->SetActorLocation(TargetLocation);
}

void UFallingTileComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//deprecated
}

void UFallingTileComponent::BeginFall()
{
	bShouldFall = true;
}