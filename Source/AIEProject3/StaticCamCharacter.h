// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MinigameCharacterBase.h"
#include "StaticCamCharacter.generated.h"

UCLASS()
class AIEPROJECT3_API AStaticCamCharacter : public AMinigameCharacterBase
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AStaticCamCharacter();

protected:
	// To add mapping context
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Move(const struct FInputActionValue& Value) override;
	//virtual void Look(const struct FInputActionValue& Value) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

};
