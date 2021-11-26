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
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// MeshComp->OnComponentHit.AddDynamic(this, &APBullet::OnHit);
	// MeshComp->OnComponentBeginOverlap.AddDynamic(this, &APBullet::OnBeginOverlap);

	RootComponent = MeshComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = MeshComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;
	
	// InitialLifeSpan = 3.0f;

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
		UGameplayStatics::ApplyRadialDamage(GetWorld(),
			Damage,
			GetActorLocation(),
			DamageRadius,
			nullptr,
			TArray<AActor*>{GetOwner()},
			GetOwner());
	}
}

void APBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	// PCore::PrintOnScreen(GetWorld(), "On Hit", 2.f);
	if(GetLocalRole() == ROLE_Authority)
	{
		// PCore::PrintOnScreen(GetWorld(), FString("On Hit ") + OtherActor->GetName(), 2.f);
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
		// PCore::PrintOnScreen(GetWorld(),  FString("On Overlap ") + OtherActor->GetName(), 2.f);
		SpawnEffect();
		Destroy();
	}
	//if(OtherActor && OtherActor != UGameplayStatics::GetPlayerPawn(GetWorld(), 0) && Cast<ACharacter>(OtherActor))
		//Destroy();
}

void APBullet::SpawnEffect_Implementation()
{
	if(GetLocalRole() < ROLE_Authority)
	{
		// PCore::PrintOnScreen(GetWorld(), "Effect Spawned", 3.f);
		if(ImpactEffect)UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, GetActorLocation());
		if(ImpactSound) UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
	}
}

// Called every frame
void APBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

