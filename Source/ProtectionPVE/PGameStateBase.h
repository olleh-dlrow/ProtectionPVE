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
protected:
	APGameStateBase();
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PGameState")
	float MaxTime = 30.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PGameState")
	float RemainTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="PGameState")
	bool bIsPaused = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PGameState")
	bool bIsEndGame = false;

	UPROPERTY(EditDefaultsOnly, Category="PGameState")
	TSubclassOf<UUserWidget> RankWidgetClass;
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	void OnGameEnd();

	void SetTimeText(UTextBlock* Text);

	void InitTimeText();
	
protected:
	UTextBlock* TimeText;

	int LastIntTime;
};

inline void APGameStateBase::InitTimeText()
{
	if(TimeText)
	{
		TimeText->SetText(FText::AsNumber(FMath::FloorToInt(MaxTime)));
	}
}
