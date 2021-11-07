// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "PSaveGame.h"
#include "RankWidget.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::OnStartButtonClicked()
{
	UUserWidget* Widget = CreateWidget(GetWorld(), NameInputWidgetClass);
	check(Widget);
	Widget->AddToViewport();
}

void UMainMenuWidget::OnRuleButtonClicked()
{
	UUserWidget* Widget = CreateWidget(GetWorld(), RuleWidgetClass);
	check(Widget);
	Widget->AddToViewport();
}

void UMainMenuWidget::OnRankButtonClicked()
{
	UUserWidget* Widget = CreateWidget(GetWorld(), RankWidgetClass);
	check(Widget);
	URankWidget* RW = Cast<URankWidget>(Widget);
	check(RW);
	
	USaveGame* SG = UGameplayStatics::LoadGameFromSlot("PlayerDatas", 0);
	if(SG)
	{
		UPSaveGame* GameSaver = Cast<UPSaveGame>(SG);
		if(GameSaver)
		{
			RW->UpdateRankData(GameSaver, TEXT("None"), 1);
		}
	}
	Widget->AddToViewport();
}

void UMainMenuWidget::OnQuitButtonClicked()
{
	UUserWidget* Widget = CreateWidget(GetWorld(), QuitGameWidgetClass);
	check(Widget);
	Widget->AddToViewport();

}
