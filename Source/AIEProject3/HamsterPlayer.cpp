// Fill out your copyright notice in the Description page of Project Settings.


#include "HamsterPlayer.h"

// Sets default values
AHamsterPlayer::AHamsterPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHamsterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHamsterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHamsterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

