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
		// ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
		// spawn the projectile at the muzzle
		if(BulletClass)
		{
			// FString Out = "SpawnBullet\nX: " + FString::SanitizeFloat(SpawnLocation.X)
			// 			+ " Y: " + FString::SanitizeFloat(SpawnLocation.Y)
			// 			+ " Z: " + FString::SanitizeFloat(SpawnLocation.Z);
			// PCore::PrintOnScreen(GetWorld(), Out, 4.f);
			APBullet* Bullet = GetWorld()->SpawnActor<APBullet>(BulletClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if(Bullet)
			{
				Bullet->SetOwner(GetOwner());
				Bullet->SetDamage(Damage);				
			}
		}
	}
}
