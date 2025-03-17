// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingTileComponent.h"

// Sets default values for this component's properties
UFallingTileComponent::UFallingTileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFallingTileComponent::BeginPlay()
{
	Super::BeginPlay();

	//Store reference to the owning actor
	OwningActor = GetOwner();

	//Break if nullptr
	if(!OwningActor) return;

	//Checks that the owning actor has physics enabled
	if(!OwningActor->GetRootComponent()->IsSimulatingPhysics())
	{
		//tell the designer they are silly and need to enable physics 
		UE_LOG(LogTemp, Warning, TEXT("Object %s requires physics to be enabled for FallingTile to run."), *OwningActor->GetName());
		return;
	}

	UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(OwningActor->GetRootComponent());
	if (!Primitive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Casting %s to Primitive failed"), *OwningActor->GetName());
	}
	
	//Enables hit events
	Primitive->SetNotifyRigidBodyCollision(true);
	
}


// Called every frame
void UFallingTileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Break if actor is null
	if (!OwningActor) return;

	if (bShouldFall)
	{
		Fall(DeltaTime);
	}
}

void UFallingTileComponent::Fall(float DeltaTime)
{
	FVector TargetLocation = OwningActor->GetActorLocation();
	TargetLocation.Z -= DeltaTime * FallSpeed;


	OwningActor->SetActorLocation(TargetLocation);
}

void UFallingTileComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{

}