// Fill out your copyright notice in the Description page of Project Settings.


#include "RankWidget.h"

#include "Kismet/GameplayStatics.h"
#include "PSaveGame.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"


void URankWidget::OnCLoseButtonClicked()
{
	RemoveFromParent();
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if(CurrentLevelName != TEXT("MainMenu"))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
	}
}

void URankWidget::UpdateRankData(UPSaveGame* RankDataSaver, const FString& CurrentName, float CurrentScore,
	TArray<UTextBlock*>& NameTexts, TArray<UTextBlock*>& ScoreTexts, UTextBlock* MyRank, UEditableText* MyName,
	UTextBlock* MyScore)
{
	int Rank = RankDataSaver->GetRank(CurrentName);
	
	MyRank->SetText(FText::AsNumber(Rank));
	MyScore->SetText(FText::AsNumber(FMath::FloorToInt(CurrentScore)));
	MyName->SetText(FText::FromString(CurrentName));

	int Len = RankDataSaver->GetPlayerDatasLength();
	for(int i = 0; i < 5; ++i)
	{
		// RankData中存在该排名
		if(i < Len)
		{
			FString PlayerName = RankDataSaver->GetPlayerNameWithScoreSorted(i);
			float PlayerScore = RankDataSaver->GetPlayerScoreSorted(i);

			NameTexts[i]->SetText(FText::FromString(PlayerName));
			ScoreTexts[i]->SetText(FText::AsNumber(FMath::FloorToInt(PlayerScore)));
		}
		else
		{
			NameTexts[i]->SetText(FText::FromString(TEXT("None")));
			ScoreTexts[i]->SetText(FText::AsNumber(0));
		}
	}
}


