// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PPlayerState.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API APPlayerState : public APlayerState
{
	GENERATED_BODY()
protected:
	UPROPERTY(Replicated, VisibleAnywhere, Category="PPlayerState")
	int KillCount;

	UPROPERTY(Replicated, VisibleAnywhere, Category="PPlayerState")
	int DeathCount;

	UPROPERTY()
	UTextBlock* ScoreText;
public:
	// getter / setter
	UFUNCTION(BlueprintCallable)
	int GetKillCount() const {return KillCount;}

	UFUNCTION(BlueprintCallable)
	int GetDeathCount() const {return DeathCount;}

	UFUNCTION(BlueprintCallable)
	void SetKillCount(int Value) {KillCount = Value; }

	UFUNCTION(BlueprintCallable)
	void SetDeathCount(int Value) {DeathCount = Value;}
	
	UFUNCTION(BlueprintCallable)
	void SetPlayerScore(float NewScore);

	void SetScoreText(UTextBlock* Text);

	UFUNCTION()
	void CreateWeapon(int Slot, TSubclassOf<class APWeapon> WeaponClass, FName SocketName);

	UFUNCTION()
	TSubclassOf<APWeapon> GetWeaponClass(const FString& WeaponName);

	UFUNCTION()
	FName GetWeaponAttachSocketName(const FString& WeaponName);
protected:
	virtual void BeginPlay() override;
};

