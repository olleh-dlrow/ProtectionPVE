// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputPlayerNameWidget.generated.h"

class UEditableTextBox;
/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API UInputPlayerNameWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void OnButtonOKClicked();
};
