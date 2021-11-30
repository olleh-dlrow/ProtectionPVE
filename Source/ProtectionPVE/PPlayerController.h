// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "PPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API APPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	UPROPERTY(Replicated)
	bool bPossessed = false;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WidgetClass")
	TSubclassOf<UUserWidget> MainSceneWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TouchInterface")
	UTouchInterface* TouchInterface;

	virtual void OnPossess(APawn* InPawn) override;
protected:
	// 客户端的Simulate不会执行
	virtual void BeginPlay() override;

	// 客户端的Simulate不会执行
	virtual void Tick(float DeltaSeconds) override;
};
