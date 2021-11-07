// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class APWeapon;

UCLASS()
class PROTECTIONPVE_API APCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void DoTouchPressed(ETouchIndex::Type FingerIndex, FVector Location);

	void DoTouchRepeat(ETouchIndex::Type FingerIndex, FVector Location);
	
	FVector TouchStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "PCharacter")
	float TurnSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	// weapon 
	UPROPERTY(VisibleDefaultsOnly, Category = "PCharacter")
	FName RifleAttachSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "PCharacter")
	TSubclassOf<APWeapon> RifleWeaponClass;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void ChangeWalkToSprint();

	UFUNCTION()
	void ChangeSprintToWalk();

	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHit(UPARAM(ref) const FHitResult& Hit);
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PCharacter")
	float MaxWalkSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PCharacter")
	float MaxSprintSpeed = 1200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PCharacter")
	float Accelaration = 20.0f;

	// 是否能够冲刺？
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PCharacter")
	bool bCanSprint = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PCharacter")
	bool bOpenMouseTurn = false;

	// 当前武器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PCharacter")
	APWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PCharacter")
	USoundBase* ExplosionSound;

private:
	float DesiredMaxSpeed;

	void SprintTick();
};
