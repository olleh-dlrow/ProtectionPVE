// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseGameWidget.generated.h"

class UInGameSettingsWidget;

/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API UPauseGameWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="WidgetClass")
	TSubclassOf<UInGameSettingsWidget> SettingsWC;

protected:
	UFUNCTION(BlueprintCallable)
	void OnResumeButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnBackButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnSettingsButtonClicked();
};
