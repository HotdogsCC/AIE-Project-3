// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticCamCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AStaticCamCharacter::AStaticCamCharacter()
{
 	
}

// Called when the game starts or when spawned
void AStaticCamCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AStaticCamCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMinigameCharacterBase::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AStaticCamCharacter::Move);

		// Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AMinigameCharacterBase::Dash);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

// Called every frame
void AStaticCamCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStaticCamCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// get forward vector
		const FVector ForwardDirection = {1, 0 ,0};
	
		// get right vector 
		const FVector RightDirection = {0, 1 ,0};

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}

	float RotateDelta = Controller->GetCharacter()->GetVelocity().Length() * -0.573248f * UGameplayStatics::GetWorldDeltaSeconds(this);
	if (BallComponent != nullptr)
	{
		FRotator BallRotation;
		BallRotation.MakeFromEuler({ 0.0f, RotateDelta, 0.0f });
		//Rotate ball mesh
		BallComponent->AddLocalRotation(BallRotation);
	}
}


