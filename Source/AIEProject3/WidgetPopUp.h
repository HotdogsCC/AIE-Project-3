// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WidgetPopUp.generated.h"

class UPopUpUserWidget;

UCLASS()
class AIEPROJECT3_API AWidgetPopUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWidgetPopUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category="Characteristics")
	bool bDisplayOnAwake = true;
	
	//When set to true, the widget will construct on the screen, otherwise in each player's screen
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite ,Category="Characteristics")
	bool bEntireDisplay = false;
	
	UPROPERTY(EditInstanceOnly, Category="Characteristics")
	FString PopUpText = TEXT("Change me!");
	
	UPROPERTY(EditInstanceOnly, Category="Characteristics")
	float AliveTime = 3.0f;;
	
	UPROPERTY(EditDefaultsOnly, Category="Dont Touch")
	TSubclassOf<UUserWidget> SplitscreenWidget;
	UPROPERTY(EditDefaultsOnly, Category="Dont Touch")
	TSubclassOf<UUserWidget> EntireScreenWidget;

	UFUNCTION(BlueprintCallable)
	void CreatePlayerWidgets();
	
	void CreateMyWidget(int32 playerIndex);

private:
	//Function called after x amount of time to remove widget
	void RemoveWidget();
	FTimerHandle DeathTimerHandle;

	//Assigns the text set by the designer to the blueprint
	UPopUpUserWidget* AssignText(UUserWidget* WidgetInstance, int32 PlayerIndex);
	UPopUpUserWidget* CurrentWidgetInstance[4] = {nullptr, nullptr,nullptr,nullptr};

};
