// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PWeapon.generated.h"

class USkeletalMeshComponent;

USTRUCT(BlueprintType)
struct PROTECTIONPVE_API FAimHitInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FVector TraceFrom;

	UPROPERTY()
	FVector TraceTo;
	
	UPROPERTY()
	FVector HitLocation;

	UPROPERTY()
	bool bHitHappened;

	UPROPERTY()
	FVector SpawnLocation;

	UPROPERTY()
	FRotator SpawnRotation;
};

UENUM(BlueprintType)
enum class EWeapon: uint8
{
	Rifle,
	GrenadeLauncher
};

UCLASS()
class PROTECTIONPVE_API APWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APWeapon();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName MagazineSocketName;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	int RemainBulletCount;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	int MaxBulletCount;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	bool bHasAim;

	UPROPERTY(Replicated, EditAnywhere, Category="Weapon")
	bool bCanSee;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName LeftHandIKSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;
protected:
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	EWeapon Type;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float Damage = 10.f;

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// 只在服务端执行
	UFUNCTION()
	void Shoot(FAimHitInfo Info);
	
	UFUNCTION()
	EWeapon GetType() const {return Type;}

	UFUNCTION()
	float GetDamage() const {return Damage;}

	// 只在服务端执行
	virtual void NativeShoot(FAimHitInfo Info);
	
	bool CheckAimHit(FHitResult& Hit) const;
};

