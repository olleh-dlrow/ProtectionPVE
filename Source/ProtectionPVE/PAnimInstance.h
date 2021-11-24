// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API UPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="PAnim")
	float Direction;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="PAnim")
	float Speed;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="PAnim")
	float AimYaw;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="PAnim")
	float AimPitch;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="PAnim")
	FTransform LeftHandTransform;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="PAnim")
	bool bDied;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="PAnim")
	bool bIsInAir;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="PAnim")
	bool bIsCrouch;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="PAnim")
	float ShootWeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PAnim")
	float ShootWeightSpeed = 8.f;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="PAnim")
	float HandIKWeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PAnim")
	float HandIKWeightSpeed = 8.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PAnim")
	class APCharacter* PCharacter;
protected:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	void SetAnimParams(float DeltaTime);

	void SnapLeftHandToWeapon();
};
