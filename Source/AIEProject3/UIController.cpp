// Fill out your copyright notice in the Description page of Project Settings.


#include "UIController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MainGameInstance.h"

// Sets default values
AUIController::AUIController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUIController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Selecting
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Started, this, &AUIController::Select);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void AUIController::Select()
{
	if(!GetWorld()) return;
	
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	if(!GameInstance) return;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(!PlayerController) return;

	//tells the game instance that PlayerX, where X is the number, joined
	GameInstance->PlayerJoined(PlayerController->GetLocalPlayer()->GetControllerId());

	UpdateLobbyScreenWidget(PlayerController->GetLocalPlayer()->GetControllerId());
}



