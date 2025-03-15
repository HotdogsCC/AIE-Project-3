// Fill out your copyright notice in the Description page of Project Settings.


#include "PopUpUserWidget.h"

void UPopUpUserWidget::SetText(FString InString)
{
	DisplayText->SetText(FText::FromString(InString));
}
