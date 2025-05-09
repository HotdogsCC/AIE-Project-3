// Fill out your copyright notice in the Description page of Project Settings.


#include "MinigameCharacterBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MinigameGameModeBase.h"

// Sets default values
AMinigameCharacterBase::AMinigameCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Create the ball
	BallComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HampterBall"));
	BallComponent->SetupAttachment(RootComponent);

	//set up collision delegate
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMinigameCharacterBase::NotifyHit);

}

// Called when the game starts or when spawned
void AMinigameCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinigameCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMinigameCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	

}

void AMinigameCharacterBase::NotifyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//try to cast to player
	AMinigameCharacterBase* OtherCharacter = Cast<AMinigameCharacterBase>(OtherActor);
	if (OtherCharacter && bShouldBounce)
	{
		//make sure we don't bounce for a little bit
		//i.e. prevent infinite loop
		bShouldBounce = false;

		//reset bShouldBounce to true in 0.1 secs
		GetWorld()->GetTimerManager().SetTimer(
			BounceResetTimer, this, &AMinigameCharacterBase::ResetBounce, 0.1f);

		//calc a force for bounce
		FVector FromOtherToThis = GetActorLocation() - OtherCharacter->GetActorLocation();
		FromOtherToThis.Normalize();
		FromOtherToThis *= 1000000.0f;
		FromOtherToThis *= Bounciness;

		//set force on this
		GetCharacterMovement()->AddForce(FromOtherToThis);

		//sometimes unreal engine is a piece of shit and decides it doesn't want to
		//say that there was a collision for the other actor.
		//in this case, we will manually do it
		if (OtherCharacter->GetShouldBounce())
		{
			OtherCharacter->NotifyHit(HitComp, this, nullptr, NormalImpulse, Hit);
		}
		//if this else condition triggers, it means that both characters have successfully processed
		//their collision! yippee!
		//in this case, we need to let the game mode know that a collision occured
		else
		{
			//get the game mode
			AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);
			if (!GameModeBase)
			{
				UE_LOG(LogTemp, Warning, TEXT("Game Mode not found for AIEProject3Character NotifyHit()"));
				return;
			}

			//turn it into a minigame game mode
			AMinigameGameModeBase* MinigameGameMode = Cast<AMinigameGameModeBase>(GameModeBase);
			if (!MinigameGameMode)
			{
				UE_LOG(LogTemp, Warning, TEXT("Minigame Game Mode not found for AIEProject3Character NotifyHit()"));
				return;
			}

			//tell the minigame a collision occured
			MinigameGameMode->PlayerCollision(this, OtherCharacter);

		}
	}

}

void AMinigameCharacterBase::ResetBounce()
{
	bShouldBounce = true;
}

void AMinigameCharacterBase::Move(const struct FInputActionValue& Value)
{
	UE_LOG(LogTemp, Error, TEXT("You need to assign Move not in the character base"));
}
void AMinigameCharacterBase::Look(const struct FInputActionValue& Value)
{
	UE_LOG(LogTemp, Error, TEXT("You need to assign Look not in the character base"));
}