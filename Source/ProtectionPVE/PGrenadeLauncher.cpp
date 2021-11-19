// Fill out your copyright notice in the Description page of Project Settings.


#include "PGrenadeLauncher.h"

#include "PBullet.h"


void APGrenadeLauncher::NativeShoot()
{
	Super::NativeShoot();

	check(PC)
	const FRotator SpawnRotation = MeshComp->GetSocketRotation(MuzzleSocketName);
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
	// spawn the projectile at the muzzle
	if(BulletClass)
	{
		APBullet* Bullet = GetWorld()->SpawnActor<APBullet>(BulletClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		Bullet->SetImpactEffect(DefaultImpactEffect);
	}
}
