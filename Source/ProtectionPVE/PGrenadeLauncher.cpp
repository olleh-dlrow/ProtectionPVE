// Fill out your copyright notice in the Description page of Project Settings.

#include "PGrenadeLauncher.h"

#include "PBullet.h"
#include "PCore.h"


void APGrenadeLauncher::NativeShoot(FAimHitInfo Info)
{
	Super::NativeShoot(Info);

	if(GetLocalRole() == ROLE_Authority)
	{
		const FRotator SpawnRotation = Info.SpawnRotation;
		const FVector SpawnLocation = Info.SpawnLocation;
	
		FActorSpawnParameters ActorSpawnParams;
	
		// spawn the projectile at the muzzle
		if(BulletClass)
		{
			APBullet* Bullet = GetWorld()->SpawnActor<APBullet>(BulletClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if(Bullet)
			{
				Bullet->SetOwner(GetOwner());
				Bullet->SetDamage(Damage);				
			}
		}
	}
}
