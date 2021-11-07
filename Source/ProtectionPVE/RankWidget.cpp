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

void URankWidget::UpdateRankData(UPSaveGame* RankDataSaver,const FString CurrentName, float CurrentScore)
{
	// 获取控件中的变量
	UTextBlock* MyRank = Cast<UTextBlock>(GetWidgetFromName(TEXT("MyRank")));
	UTextBlock* MyScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("MyScore")));
	UEditableText* MyName = Cast<UEditableText>(GetWidgetFromName(TEXT("MyName")));
	check(MyRank);
	check(MyScore);
	check(MyName);

	TArray<UTextBlock*> NameTexts;
	NameTexts.Init(nullptr, 5);

	TArray<UTextBlock*> ScoreTexts;
	ScoreTexts.Init(nullptr, 5);
	
	for(int i = 0; i < 5; ++i)
	{
		const FString Name = "Name" + FString::FromInt(i + 1);
		const FString Score = "Score" + FString::FromInt(i + 1);
		NameTexts[i] = Cast<UTextBlock>(GetWidgetFromName(FName(Name)));
		ScoreTexts[i] = Cast<UTextBlock>(GetWidgetFromName(FName(Score)));
		check(NameTexts[i]);
		check(ScoreTexts[i]);
	}
	
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


