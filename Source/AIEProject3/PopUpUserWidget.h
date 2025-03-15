// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PopUpUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AIEPROJECT3_API UPopUpUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayText;

	//Sets the text of the widget
	void SetText(FString InString);
};
