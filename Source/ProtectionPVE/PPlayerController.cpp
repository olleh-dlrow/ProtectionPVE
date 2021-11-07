// Fill out your copyright notice in the Description page of Project Settings.


#include "PPlayerController.h"

#include "PGameInstance.h"
#include "PGameStateBase.h"
#include "PPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void APPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* MainScene = CreateWidget(this, MainSceneWidgetClass);
	
	if(MainScene)
	{
		// 进入游戏时设置名称
		UTextBlock* NameText = Cast<UTextBlock>(MainScene->GetWidgetFromName(TEXT("NameText")));
		if(NameText)
		{
			UPGameInstance* GI = Cast<UPGameInstance>(GetGameInstance());
			if(GI)
			{
				NameText->SetText(FText::FromString(GI->PlayerName));
			}
		}
		
		APGameStateBase* GameState = Cast<APGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
		if(GameState)
		{
			UTextBlock* TimeText = Cast<UTextBlock>(MainScene->GetWidgetFromName(TEXT("TimeText")));
			GameState->SetTimeText(TimeText);
			GameState->InitTimeText();
		}
		
		APPlayerState* PS = GetPlayerState<APPlayerState>();
		if(PS)
		{
			UTextBlock* ScoreText = Cast<UTextBlock>(MainScene->GetWidgetFromName(TEXT("ScoreText")));
			PS->SetScoreText(ScoreText);
		}
		
		MainScene->AddToViewport();
	}
}
