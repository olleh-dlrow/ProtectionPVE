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

	void SetImpactEffect(class UParticleSystem* Effect);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	
	UPROPERTY(EditDefaultsOnly, Category="Bullet")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY()
	class UParticleSystem* ImpactEffect;
};

