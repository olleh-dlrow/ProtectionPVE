// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PSaveGame.generated.h"

USTRUCT()
struct FPlayerData
{
	GENERATED_BODY()

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	float PlayerScore;

	FPlayerData(const FString& Name, float Score);

	FPlayerData();
};

/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API UPSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FPlayerData> PlayerDatas;

	UFUNCTION(BlueprintCallable)
	int GetPlayerDatasLength() const;
	
	UFUNCTION(BlueprintCallable)
	void SetPlayerData(const FString& NewPlayerName, float NewPlayerScore);

	UFUNCTION(BlueprintCallable)
	FString GetPlayerNameWithScoreSorted(int Index);

	UFUNCTION(BlueprintCallable)
	float GetPlayerScoreSorted(int Index);

	// 获取玩家名称对应的排名
	UFUNCTION(BlueprintCallable)
	int GetRank(const FString& Name);
	
};
