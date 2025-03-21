// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FallingTileComponent.generated.h"

UENUM(BlueprintType)
enum class EFallMode : uint8 {
	ConstantSpeed = 0 UMETA(DisplayName = "Constant Speed"),
	Acceleration = 1 UMETA(DisplayName = "Acceleration")
};
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

	//When enabled, shows debug lines for the collision zone
	UPROPERTY(EditInstanceOnly, Category="Debugging")
	bool bDebugMode = true;
	
	//Seconds between a player touching the tile, and falling
	UPROPERTY(EditInstanceOnly, Category="Important")
	float WaitTime = 1.0f;

	UPROPERTY(EditInstanceOnly, Category="Important")
	EFallMode FallMode = EFallMode::ConstantSpeed;
	
	//Controls the units per second the tile falls fore
	UPROPERTY(EditInstanceOnly, Category="Constant Speed")
	float FallSpeed = 10.0f;

	UPROPERTY(EditInstanceOnly, Category="Acceleration")
	float InitialFallSpeed = 100.0f;
	
	UPROPERTY(EditInstanceOnly, Category="Acceleration")
	float FallSpeedAcceleration = 100.0f;

	//How far along the z axis the trace should occur
	UPROPERTY(EditInstanceOnly, Category="Important")
	float CollisionCenterOffset = 50.0f;

	//The radius of the collision zone check
	UPROPERTY(EditInstanceOnly, Category="Important")
	float CollisionSphereRadius = 40.0f;
	
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
