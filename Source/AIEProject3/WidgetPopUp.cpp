// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetPopUp.h"

#include "PopUpUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWidgetPopUp::AWidgetPopUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWidgetPopUp::BeginPlay()
{
	Super::BeginPlay();

	if(!GetWorld()) return;
	
	if(bDisplayOnAwake)
	{
		CreatePlayerWidgets();
	}
	
}

// Called every frame
void AWidgetPopUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UPopUpUserWidget* AWidgetPopUp::AssignText(UUserWidget* WidgetInstance, int32 PlayerIndex)
{
	if(!WidgetInstance) return nullptr;
	
	CurrentWidgetInstance[PlayerIndex] = Cast<UPopUpUserWidget>(WidgetInstance);
	if(!CurrentWidgetInstance) return nullptr;

	CurrentWidgetInstance[PlayerIndex]->SetText(PopUpText);
	return CurrentWidgetInstance[PlayerIndex];
}

void AWidgetPopUp::CreateMyWidget(int32 playerIndex)
{

	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), playerIndex);
	
	if(AliveTime > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &AWidgetPopUp::RemoveWidget, AliveTime);
	}
	
	UUserWidget* WidgetInstance; 
	//Assigns correct widget for splitscreen or entire display
	if(bEntireDisplay)
	{
		WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), EntireScreenWidget); 

		//Casts to PopUpUserWidget, sets text, checks everything went okay
		if(UPopUpUserWidget* CastedWidget = AssignText(WidgetInstance, playerIndex))
		{
			CastedWidget->AddToViewport();
		}
	}
	else
	{
		WidgetInstance = CreateWidget<UUserWidget>(PlayerController, SplitscreenWidget);

		//Casts to PopUpUserWidget, sets text, checks everything went okay
		if(UPopUpUserWidget* CastedWidget = AssignText(WidgetInstance, playerIndex))
		{
			CastedWidget->AddToPlayerScreen();
		}
	}
}

void AWidgetPopUp::RemoveWidget()
{
	for(int i = 0; i < 4; i++)
	{
		if(CurrentWidgetInstance[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("poo"));
			CurrentWidgetInstance[i]->RemoveFromParent();
		}
	}
	
}

void AWidgetPopUp::CreatePlayerWidgets()
{
	int32 players = UGameplayStatics::GetNumPlayerControllers(this);
	UE_LOG(LogTemp, Warning, TEXT("Players: %i"),players);

	for(int32 i = 0; i < UGameplayStatics::GetNumPlayerControllers(this); i++)
	{
		CreateMyWidget(i);
	}
}
