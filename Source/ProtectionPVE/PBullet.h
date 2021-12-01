// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PBullet.generated.h"

UCLASS()
class PROTECTIONPVE_API APBullet : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	APBullet();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Bullet")
	float Damage = 15.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Bullet")
	float DamageRadius = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Bullet")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category="Bullet")
	class UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category="Bullet")
	USoundBase* ImpactSound;
	
public:
	UFUNCTION()
	void SetDamage(float Value) {Damage = Value;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;
	
	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(NetMulticast, Reliable)
	void SpawnEffect();
};



