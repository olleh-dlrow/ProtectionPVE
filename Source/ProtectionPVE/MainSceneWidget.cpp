// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSceneWidget.h"

#include "PCharacter.h"
#include "PGameInstance.h"
#include "PGameStateBase.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UMainSceneWidget::OnWidgetConstructed()
{
	Character = Cast<APCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UPGameInstance* GI = Cast<UPGameInstance>(GetGameInstance());
	check(GI);
	
	// 设置玩家名称
	UTextBlock* NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));
	NameText->SetText(FText::FromString(GI->PlayerName));
}

void UMainSceneWidget::OnPauseButtonClicked()
{
	Character->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	APGameStateBase* GameState = Cast<APGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if(GameState)
	{
		GameState->bIsPaused = true;
		UUserWidget* PauseWidget = CreateWidget(this, PauseWidgetClass);
		PauseWidget->AddToViewport();
	}
	
}

void UMainSceneWidget::OnFireButtonClicked()
{
	Character->Fire();
}
