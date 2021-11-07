// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainSceneWidget.generated.h"

class APCharacter;

/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API UMainSceneWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category="WidgetClass")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(VisibleAnywhere, Category="Ref")
	APCharacter* Character;

	UFUNCTION(BlueprintCallable)
	void OnWidgetConstructed();
	
	UFUNCTION(BlueprintCallable)
	void OnPauseButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnFireButtonClicked();
};
