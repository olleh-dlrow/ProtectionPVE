// Fill out your copyright notice in the Description page of Project Settings.


#include "PGameStateBase.h"

#include "PGameInstance.h"
#include "PSaveGame.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "RankWidget.h"
#include "Net/UnrealNetwork.h"
#include "Components/TextBlock.h"

APGameStateBase::APGameStateBase()
{

}

void APGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	RemainTime = MaxTime;
	LastIntTime = FMath::FloorToInt(RemainTime);
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

void APGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(APGameStateBase, RemainTime);
	DOREPLIFETIME(APGameStateBase, TotalDeathCount);
}
