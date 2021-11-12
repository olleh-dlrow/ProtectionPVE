// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainSceneWidget.h"

#include "PGameInstance.generated.h"

class UMainSceneWidget;
/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API UPGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PGameInstance")
	FString PlayerName = "Default";

	UPROPERTY(EditDefaultsOnly, Category="PGameInstance")
	FString MainLevelName = "MainLevel";
};
