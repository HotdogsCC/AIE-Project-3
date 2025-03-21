// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FallingTileComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIEPROJECT3_API UFallingTileComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFallingTileComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Seconds between a player touching the tile, and falling
	UPROPERTY(EditInstanceOnly)
	float WaitTime = 1.0f;

	//Controls the units per second the tile falls fore
	UPROPERTY(EditInstanceOnly)
	float FallSpeed = 10.0f;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	//Reference to the actor owning this component
	AActor* OwningActor;

	//When true, a player collision has occured
	bool bPlayerTouched = false;

	//When true, the platform will start falling
	bool bShouldFall = false;

	//Timer handle for handling time between toouching the tile and the tile falling
	FTimerHandle FallTimerHandle;

	//Handles falling logic
	void Fall(float DeltaTime);

	//Runs when the tile should start falling
	void BeginFall();

	void DoBoxTrace();

};
