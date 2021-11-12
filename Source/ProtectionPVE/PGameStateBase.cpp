// Fill out your copyright notice in the Description page of Project Settings.


#include "PGameStateBase.h"

#include "PGameInstance.h"
#include "PSaveGame.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "RankWidget.h"
#include "Components/TextBlock.h"

APGameStateBase::APGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	RemainTime = MaxTime;
	LastIntTime = FMath::FloorToInt(RemainTime);
}

void APGameStateBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(RemainTime > 0 && !bIsPaused)
	{
		// RemainTime -= DeltaSeconds;
		int CurrentIntTime = FMath::FloorToInt(RemainTime);
		if(CurrentIntTime < LastIntTime)
		{
			LastIntTime = CurrentIntTime;
			if(TimeText)
			{
				FString Time = "00:";
				if(LastIntTime / 10 == 0)
				{
					Time += "0" + FString::FromInt(LastIntTime);
				}
				else
				{
					Time += FString::FromInt(LastIntTime);
				}
				TimeText->SetText(FText::FromString(Time));
			}
		}
	}
	else if(RemainTime <= 0 && !bIsEndGame)
	{
		TimeText->SetText(FText::AsNumber(0));
		
		OnGameEnd();
		bIsEndGame = true;
	}
}

void APGameStateBase::OnGameEnd()
{
	UPGameInstance* GI = Cast<UPGameInstance>(GetGameInstance());
	if(GI)
	{
		USaveGame* SG = UGameplayStatics::LoadGameFromSlot("PlayerDatas", 0);
		if(SG)
		{
			UPSaveGame* GameSaver = Cast<UPSaveGame>(SG);
			// 不存在一个GameSaver
			if(!GameSaver)
			{
				GameSaver = Cast<UPSaveGame>(UGameplayStatics::CreateSaveGameObject(UPSaveGame::StaticClass()));
				check(GameSaver);
			}

			// 获取和保存数据
			APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			float Score = Pawn->GetPlayerState()->GetScore();
			
			GameSaver->SetPlayerData(GI->PlayerName, Score);

			UGameplayStatics::SaveGameToSlot(GameSaver, "PlayerDatas", 0);

			// 创建排行榜并更新数据
			URankWidget* RankWidget = CreateWidget<URankWidget>(GetWorld(), RankWidgetClass);
			if(RankWidget)
			{
				RankWidget->UpdateRankData(GameSaver, GI->PlayerName, Score);
				RankWidget->AddToViewport();
			}
		}
	}
}

void APGameStateBase::SetTimeText(UTextBlock* Text)
{
	TimeText = Text;
}
