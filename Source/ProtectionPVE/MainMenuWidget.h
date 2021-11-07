// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="WidgetClass")
	TSubclassOf<UUserWidget> NameInputWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category="WidgetClass")
	TSubclassOf<UUserWidget> RuleWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category="WidgetClass")
	TSubclassOf<UUserWidget> RankWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category="WidgetClass")
	TSubclassOf<UUserWidget> QuitGameWidgetClass;
	
	UFUNCTION(BlueprintCallable)
	void OnStartButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnRuleButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnRankButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnQuitButtonClicked();
};
