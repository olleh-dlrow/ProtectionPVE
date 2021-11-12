// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "PPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API APPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WidgetClass")
	TSubclassOf<UUserWidget> MainSceneWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TouchInterface")
	UTouchInterface* TouchInterface;
};
