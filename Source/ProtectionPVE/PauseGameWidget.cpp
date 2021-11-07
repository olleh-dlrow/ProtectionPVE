// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseGameWidget.h"

#include "PGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "InGameSettingsWidget.h"

void UPauseGameWidget::OnResumeButtonClicked()
{
	APGameStateBase* GS = GetWorld()->GetGameState<APGameStateBase>();
	if(GS)
	{
		GS->bIsPaused = false;
		APawn* Pawn = GetOwningPlayerPawn();
		Pawn->EnableInput(nullptr);
	}
	RemoveFromParent();
}

void UPauseGameWidget::OnBackButtonClicked()
{
	RemoveFromParent();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
}

void UPauseGameWidget::OnSettingsButtonClicked()
{
	UUserWidget* SettingsWidget = CreateWidget(this, SettingsWC);
	if(SettingsWidget)
	{
		SettingsWidget->AddToViewport();
	}
}


