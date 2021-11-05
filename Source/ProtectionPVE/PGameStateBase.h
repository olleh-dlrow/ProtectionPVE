// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API APGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PGameState")
	float MaxTime = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PGameState")
	float RemainTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PGameState")
	bool bIsPaused = false;
};
