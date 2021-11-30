// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PWeapon.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

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

	UFUNCTION(BlueprintCallable)
	void OnDestroy();
	
	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnBody(float Value);
	
	void LookUp(float Value);
	
	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void DoTouchPressed(ETouchIndex::Type FingerIndex, FVector Location);

	void DoTouchRepeat(ETouchIndex::Type FingerIndex, FVector Location);

	UFUNCTION(BlueprintCallable)
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupt);
	
	bool CheckAimHit(FHitResult& Hit);

	// 计算发射轨迹线的射线
	void CalculateTraceEnds(FVector& TraceFrom, FVector& TraceTo);
	
	bool CheckWeaponIndex(int Index) const;

	UPROPERTY()
	FAimHitInfo AimHitInfo;
	
	UPROPERTY(Replicated)
	TArray<APWeapon*> Weapons = {nullptr, nullptr};

	UPROPERTY(Replicated)
	int CurrentWeaponIndex = -1;
	
	FVector LastTouchLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "PCharacter")
	float TurnSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	class UPHealthComponent* HealthComp;
	
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

	UPROPERTY(EditDefaultsOnly, Category="PCharacter")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category="PCharacter")
	UAnimMontage* HitMontage;
	
	UPROPERTY()
	UMainSceneWidget* MainSceneWidget;

	UPROPERTY(Replicated, VisibleAnywhere, Category="PCharacter")
	int ReviveTime;

	UPROPERTY(EditDefaultsOnly, Category="PCharacter")
	int DefaultReviveTime = 5;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server, Reliable)
	void ChangeWalkToSprint();

	UFUNCTION(Server, Reliable)
	void ChangeSprintToWalk();

	UFUNCTION()
	void StartFire();
	
	UFUNCTION(Server, Reliable)
	void Server_Fire(FAimHitInfo Info);
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Fire(FAimHitInfo Info);

	UFUNCTION()
	void StartThrow();
	
	UFUNCTION(Server, Reliable)
	void Server_Throw(FAimHitInfo Info);
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Throw();

	UFUNCTION(BlueprintCallable)
	void UpdateCrouchState();

	UFUNCTION(Server, Reliable)
	void Server_Reload();
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Reload();

	UFUNCTION(Server, Reliable)
	void Server_SwitchWeapon(int Slot);
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void SwitchWeapon(int Slot);

	UFUNCTION()
	void SwitchWeapon0() {Server_SwitchWeapon(0);}

	UFUNCTION()
	void SwitchWeapon1() {Server_SwitchWeapon(1);}

	UFUNCTION(Server, Reliable)
	void Server_PickupWeapon();
	
	UFUNCTION(NetMulticast, Reliable)
	void PickupWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void OnDied();
	
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

	UFUNCTION(BlueprintCallable)
	float GetRemoteViewPitch() const
	{
		float Pitch = RemoteViewPitch * 360.f / 255.f;
		if(Pitch >= 0.f && Pitch < 180.f)return FMath::Clamp(Pitch, 0.f, 90.0f);
		return FMath::Clamp(Pitch - 360.f, -90.f, 0.f);
	}

	UFUNCTION()
	UAnimMontage* GetHitMontage() const {return HitMontage;}
	
	// Weapon
	UFUNCTION()
	void CreateWeapon(int Slot, TSubclassOf<APWeapon> WeaponClass, FName SocketName);
	
	UFUNCTION(BlueprintCallable)
	int GetRemainBulletCount(int Index) const;
	UFUNCTION(BlueprintCallable)
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
	UFUNCTION()
	FName GetWeaponAttachSocketName(EWeapon Type);

	// MainScene Widget
	UFUNCTION()
	UMainSceneWidget* GetMainSceneWidget() const;
	UFUNCTION()
	void SetMainSceneWidget(UMainSceneWidget* Widget);

	UFUNCTION(BlueprintCallable)
	int GetReviveTime() const {return ReviveTime;}

	UFUNCTION(BlueprintCallable)
	int GetDefaultReviveTime() const {return DefaultReviveTime;}

	UFUNCTION(BlueprintCallable)
	void SetReviveTime(int Value) {ReviveTime = Value;}
	
	UFUNCTION()
	void ReviveCountDown();

	UFUNCTION(NetMulticast, Reliable)
	void OnRevive();
	
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
	bool bPlayInPC = false;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PCharacter")
	USoundBase* PickupSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PCharacter")
	USoundBase* ReloadSound;
	
	UPROPERTY(Replicated, VisibleAnywhere, Category="PCharacter")
	APWeapon* DesiredPickupWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PCharacter")
	FTimerHandle ReviveTimerHandle;
	
	// 人物当前状态
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bInFreeView;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="State")
	bool bCanPickup;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="State")
	bool bDied;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bIsFiring;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bIsThrowing;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bIsReloading;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="State")
	float HandIKWeight;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="State")
	float ShootWeight;
private:
	UFUNCTION()
	void SprintTick();

	UPROPERTY(Replicated)
	float DesiredMaxSpeed;
};



