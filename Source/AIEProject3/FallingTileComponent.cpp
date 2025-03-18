// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingTileComponent.h"
#include "GameFramework/Character.h"

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

	//Disables gravuty
	Primitive->SetEnableGravity(false);

	//checks constraints
	if(!Primitive->BodyInstance.bLockXTranslation)
	{
		UE_LOG(LogTemp, Warning, TEXT("Object %s requires all constraints to be locked to be enabled for FallingTile to run."), *OwningActor->GetName());
		return;
	}

	Primitive->OnComponentHit.AddDynamic(this, &UFallingTileComponent::OnHit);

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
	//only check if bShouldFall should be true if it is not already true
	if(!bPlayerTouched)
	{
		//checks it is a player touching the actor
		UE_LOG(LogTemp, Display, TEXT("%s just touched %s"), *this->GetName(), *OtherActor->GetName());
		ACharacter* MyCharacter = Cast<ACharacter>(OtherActor);
		if(MyCharacter)
		{
			//starts falling
			UE_LOG(LogTemp, Display, TEXT("%s is a character, beginng fall countdown for %s"), *OtherActor->GetName(), *this->GetName());
			GetWorld()->GetTimerManager().SetTimer(FallTimerHandle, this, &UFallingTileComponent::BeginFall, WaitTime);
			bPlayerTouched = true;
		}
	}
}

void UFallingTileComponent::BeginFall()
{
	bShouldFall = true;
	UE_LOG(LogTemp, Display, TEXT("%s is falling"), *this->GetName());
}