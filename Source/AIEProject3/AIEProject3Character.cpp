// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIEProject3Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MinigameGameModeBase.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AAIEProject3Character

AAIEProject3Character::AAIEProject3Character()
{
	PrimaryActorTick.bCanEverTick = true; // Enables ticking
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create the ball
	BallComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HampterBall"));
	BallComponent->SetupAttachment(RootComponent);

	//set up collision delegate
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAIEProject3Character::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AAIEProject3Character::NotifyHit);
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AAIEProject3Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

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

	MinigameGameMode->DeclarePlayer(this);

	

	

}

void AAIEProject3Character::Tick(float DeltaTime)
{
	//very awful code but IDK how else to do it
	if(GetCapsuleComponent())
	{
		GetCapsuleComponent()->WakeRigidBody();
	}
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAIEProject3Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAIEProject3Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAIEProject3Character::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AAIEProject3Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
		
		//UE_LOG(LogTemp, Display, TEXT("%f"),Controller->GetCharacter()->GetVelocity().Length());
		float RotateDelta = Controller->GetCharacter()->GetVelocity().Length() * -0.573248f * UGameplayStatics::GetWorldDeltaSeconds(this);
		//UE_LOG(LogTemp, Display, TEXT("%f"),RotateDelta);
		if(BallComponent != nullptr)
		{
			FRotator BallRotation;
			BallRotation.MakeFromEuler({0.0f, RotateDelta, 0.0f});
			//Rotate ball mesh
			BallComponent->AddLocalRotation(BallRotation);
		}
		
	}
}

void AAIEProject3Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

//deprecated
void AAIEProject3Character::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Display, TEXT("%s touched %s"), *this->GetName(), *OtherActor->GetName());
}

void AAIEProject3Character::NotifyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//try to cast to player
	AAIEProject3Character* OtherCharacter = Cast<AAIEProject3Character>(OtherActor);
	if (OtherCharacter && bShouldBounce)
	{
		//make sure we don't bounce for a little bit
		//i.e. prevent infinite loop
		bShouldBounce = false;

		//reset bShouldBounce to true in 0.1 secs
		GetWorld()->GetTimerManager().SetTimer(
			BounceResetTimer, this, &AAIEProject3Character::ResetBounce, 0.1f);

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
		if(OtherCharacter->GetShouldBounce())
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

void AAIEProject3Character::ResetBounce()
{
	bShouldBounce = true;
}

