// Fill out your copyright notice in the Description page of Project Settings.


#include "PSaveGame.h"

FPlayerData::FPlayerData(const FString& Name, float Score):PlayerName(Name), PlayerScore(Score)
{
}

FPlayerData::FPlayerData():PlayerName(TEXT("Default")), PlayerScore(0)
{
}

int UPSaveGame::GetPlayerDatasLength() const
{
	return PlayerDatas.Num();
}

void UPSaveGame::SetPlayerData(const FString& NewPlayerName, float NewPlayerScore)
{
	for(int i = 0; i < PlayerDatas.Num(); ++i)
	{
		FPlayerData& Data = PlayerDatas[i];
		if (Data.PlayerName == NewPlayerName)
		{
			Data.PlayerScore = NewPlayerScore;
			PlayerDatas.Sort([](const FPlayerData& A, const FPlayerData& B){return A.PlayerScore > B.PlayerScore;});
			return;
		}
	}

	PlayerDatas.Add(FPlayerData(NewPlayerName, NewPlayerScore));
	PlayerDatas.Sort([](const FPlayerData& A, const FPlayerData& B){return A.PlayerScore > B.PlayerScore;});
}

FString UPSaveGame::GetPlayerNameWithScoreSorted(int Index)
{
	return PlayerDatas[Index].PlayerName;
}

float UPSaveGame::GetPlayerScoreSorted(int Index)
{
	return PlayerDatas[Index].PlayerScore;
}

int UPSaveGame::GetRank(const FString& Name)
{
	for(int i = 0; i < PlayerDatas.Num(); ++i)
	{
		FPlayerData& Data = PlayerDatas[i];
		if(Data.PlayerName == Name)
		{
			return i + 1;
		}
	}

	return 1;
}


