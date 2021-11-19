// Fill out your copyright notice in the Description page of Project Settings.


#include "PBullet.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
APBullet::APBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->OnComponentHit.AddDynamic(this, &APBullet::OnHit);

	RootComponent = MeshComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = MeshComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = true;

	InitialLifeSpan = 3.0f;
}

void APBullet::SetImpactEffect(UParticleSystem* Effect)
{
	ImpactEffect = Effect;
}

// Called when the game starts or when spawned
void APBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

inline void APBullet::Destroyed()
{
	Super::Destroyed();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, GetActorLocation());	
}

void APBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	// if(ImpactEffect)
	// 	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location);
	//
	// Destroy();
}

// Called every frame
void APBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

