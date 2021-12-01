// Fill out your copyright notice in the Description page of Project Settings.


#include "PBullet.h"

#include "PCore.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
APBullet::APBullet()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	RootComponent = MeshComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = MeshComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;
	
	InitialLifeSpan = 3.0f;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void APBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

inline void APBullet::Destroyed()
{
	Super::Destroyed();

	// 服务端判定伤害
	if(GetLocalRole() == ROLE_Authority)
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Friend", Actors);
		UGameplayStatics::ApplyRadialDamage(GetWorld(),
			Damage,
			GetActorLocation(),
			DamageRadius,
			nullptr,
			Actors,
			GetOwner());
	}
}

void APBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		SpawnEffect();
		Destroy();
	}
}

void APBullet::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		SpawnEffect();
		Destroy();
	}
}

void APBullet::SpawnEffect_Implementation()
{
	
	if(ImpactEffect)UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, GetActorLocation());
	if(ImpactSound) UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
}

