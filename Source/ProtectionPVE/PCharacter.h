// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "PCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class APWeapon;
class APGrenade;

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

	UFUNCTION(BlueprintCallable)
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupt);
	
	bool CheckAimHit(FHitResult& Hit);
	
	FVector LastTouchLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "PCharacter")
	float TurnSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	ACameraActor* FreeViewCamera;

	UPROPERTY(EditAnywhere, Category="Actor")
	FVector FVCameraOffset;

	UPROPERTY(EditAnywhere, Category="Actor")
	FRotator FVCameraRotator;
	
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	// UChildActorComponent* ChildActorComp;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	// UChildActorComponent* BackCameraChildActorComp;
	
	UPROPERTY(EditDefaultsOnly, Category="PCharacter")
	FName ThrowAttachSocketName = "ThrowSocket";

	UPROPERTY(EditDefaultsOnly, Category="PCharacter")
	TSubclassOf<APGrenade> GrenadeClass;
	
	UPROPERTY(BlueprintReadOnly, Category="PCharacter")
	APGrenade* Grenade;
	
	UPROPERTY(EditDefaultsOnly, Category="PCharacter")
	UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, Category="PCharacter")
	UAnimMontage* ThrowMontage;

	UPROPERTY(EditDefaultsOnly, Category="PCharacter")
	UAnimMontage* ReloadMontage;
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

	UFUNCTION(BlueprintCallable)
	void Throw();

	UFUNCTION(BlueprintCallable)
	void Reload();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHit(UPARAM(ref) const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void NotifyThrowOut();

	UFUNCTION(BlueprintCallable)
	void EnterFreeView();

	UFUNCTION(BlueprintCallable)
	void ExitFreeView();

	UFUNCTION()
	void SetCurrentBulletCount(int Count);

	UFUNCTION()
	void CreateWeapon(int Slot, TSubclassOf<APWeapon> WeaponClass, FName SocketName);
	
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

	UPROPERTY(EditDefaultsOnly, Category="PCharacter")
	float MaxCrouchSpeed = 300.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PCharacter")
	float Acceleration = 20.0f;

	// 投掷速度
	UPROPERTY(EditAnywhere, Category="PCharacter")
	FVector MaxThrowVelocity = FVector(1000.0f, 1000.0f, 0.0f);
	
	// 用于控制free view模式下的Yaw角
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PCharacter")
	float CurrentYaw;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PCharacter")
	float YawRotateSpeed = 2.0f;
	
	// 是否能够冲刺？
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PCharacter")
	bool bCanSprint = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PCharacter")
	bool bOpenMouseTurn = false;

	UPROPERTY(EditDefaultsOnly, Category = "PCharacter")
	TSubclassOf<APWeapon> RifleWeaponClass;

	// weapon 
	UPROPERTY(EditDefaultsOnly, Category = "PCharacter")
	FName RifleAttachSocketName = "RifleSocket";
	
	// 当前武器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PCharacter")
	APWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PCharacter")
	USoundBase* ExplosionSound;

	UPROPERTY(EditDefaultsOnly, Category="WidgetClass")
	APWeapon* Weapon1;

	UPROPERTY(EditDefaultsOnly, Category="WidgetClass")
	APWeapon* Weapon2;
	
	UTextBlock* CurrentBullet1Text;

	UTextBlock* CurrentBullet2Text;

	UTextBlock* MaxBullet1Text;

	UTextBlock* MaxBullet2Text;

	// 当前手上拿着的武器
	UTextBlock* CurrentBulletText;
	
	// 人物当前状态
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bInFreeView;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bIsFiring;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bIsThrowing;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bIsReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	float HandIKWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	float ShootWeight;
private:
	void SprintTick();
	
	float DesiredMaxSpeed;
};



