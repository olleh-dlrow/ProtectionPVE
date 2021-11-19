// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "PCharacter.generated.h"

class UMainSceneWidget;
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

	bool CheckWeaponIndex(int Index) const;

	TArray<APWeapon*> Weapons = {nullptr, nullptr};

	int CurrentWeaponIndex = -1;
	
	FVector LastTouchLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "PCharacter")
	float TurnSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	// class USceneComponent* PickupSceneComp;
	//
	// UPROPERTY(EditDefaultsOnly, Category="Components")
	// class UWidgetComponent* PickupComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	ACameraActor* FreeViewCamera;

	UPROPERTY(EditAnywhere, Category="Actor")
	FVector FVCameraOffset;

	UPROPERTY(EditAnywhere, Category="Actor")
	FRotator FVCameraRotator;
	
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

	UPROPERTY()
	UMainSceneWidget* MainSceneWidget;
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

	UFUNCTION(BlueprintCallable)
	void SwitchWeapon(int Slot);

	UFUNCTION()
	void PickupWeapon();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHit(UPARAM(ref) const FHitResult& Hit);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable)
	void NotifyThrowOut();

	UFUNCTION(BlueprintCallable)
	void EnterFreeView();

	UFUNCTION(BlueprintCallable)
	void ExitFreeView();

	// Weapon
	UFUNCTION()
	void CreateWeapon(int Slot, TSubclassOf<APWeapon> WeaponClass, FName SocketName);
	
	UFUNCTION()
	int GetRemainBulletCount(int Index) const;
	UFUNCTION()
	int GetMaxBulletCount(int Index) const;
	UFUNCTION()
	APWeapon* GetWeapon(int Index) const;

	UFUNCTION()
	void SetNewWeapon(int Index, APWeapon* NewWeapon);
	UFUNCTION()
	void SetRemainBulletCount(int Index, int Count);
	UFUNCTION()
	void SetMaxBulletCount(int Index, int Count);

	// Current Weapon
	UFUNCTION(BlueprintCallable)
	APWeapon* GetCurrentWeapon() const;
	UFUNCTION()
	void SetCurrentWeaponInSlot(int Slot);
	UFUNCTION()
	void PutBackCurrentWeapon();
	UFUNCTION()
	int GetCurrentRemainBulletCount() const;
	UFUNCTION()
	void SetCurrentRemainBulletCount(int Count);
	UFUNCTION()
	int GetCurrentMaxBulletCount() const;
	UFUNCTION()
	void SetCurrentMaxBulletCount(int Count);


	// MainScene Widget
	UFUNCTION()
	UMainSceneWidget* GetMainSceneWidget() const;
	UFUNCTION()
	void SetMainSceneWidget(UMainSceneWidget* Widget);
	
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

	UPROPERTY(EditDefaultsOnly, Category="PCharacter")
	TSubclassOf<APWeapon> GrenadeLauncherClass;
	
	// weapon 
	UPROPERTY(EditDefaultsOnly, Category = "PCharacter")
	FName RifleAttachSocketName = "RifleSocket";

	UPROPERTY(EditDefaultsOnly, Category="PCharacter")
	FName GrenadeLauncherSocketName = "GrenadeLauncherSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PCharacter")
	USoundBase* ExplosionSound;
	
	// 人物当前状态
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bInFreeView;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bIsFiring;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bIsThrowing;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bIsReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	float HandIKWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	float ShootWeight;
private:
	void SprintTick();
	
	float DesiredMaxSpeed;
};



