// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RankWidget.generated.h"

class UPSaveGame;
class UTextBlock;
class UEditableText;

/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API URankWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void OnCLoseButtonClicked();

	UFUNCTION(BlueprintCallable)
	void UpdateRankData(UPSaveGame* RankDataSaver,
		const FString CurrentName,
		float CurrentScore);
};
