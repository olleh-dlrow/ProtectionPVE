// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameStateBase.h"
#include "PGameStateBase.generated.h"

class UPSaveGame;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API APGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	APGameStateBase();
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PGameState")
	float MaxTime = 30.0f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="PGameState")
	float RemainTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="PGameState")
	bool bIsPaused = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PGameState")
	bool bIsEndGame = false;

	UPROPERTY(EditDefaultsOnly, Category="PGameState")
	TSubclassOf<UUserWidget> RankWidgetClass;

protected:
	int LastIntTime;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="PGameState")
	int TotalDeathCount = 5;
	
public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnGameEnd();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnTotalDeathCountChanged();
	
	UFUNCTION()
	void SetTotalDeathCount(int Value) {TotalDeathCount = Value; OnTotalDeathCountChanged();}

	UFUNCTION()
	int GetTotalDeathCount() const {return TotalDeathCount;}
};
