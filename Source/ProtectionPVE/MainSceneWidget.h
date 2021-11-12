// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainSceneWidget.generated.h"

class APCharacter;
class APWeapon;

/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API UMainSceneWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable)
	void OnWidgetConstructed();
	
	UFUNCTION(BlueprintCallable)
	void OnPauseButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnFireButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnJumpButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnCrouchButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnThrowButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnWeaponSlot1Clicked();

	UFUNCTION(BlueprintCallable)
	void OnReloadButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnFreeViewButtonClicked();
	
	UPROPERTY(EditDefaultsOnly, Category="WidgetClass")
	TSubclassOf<UUserWidget> PauseWidgetClass;
	
	UPROPERTY(VisibleAnywhere, Category="Ref")
	APCharacter* Character;
};
