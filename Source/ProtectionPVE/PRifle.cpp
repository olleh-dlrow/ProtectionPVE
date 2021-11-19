// Fill out your copyright notice in the Description page of Project Settings.


#include "PRifle.h"

#include "Kismet/GameplayStatics.h"

void APRifle::NativeShoot()
{
	Super::NativeShoot();
	
	// 命中检测
	FHitResult Hit;
	if(CheckAimHit(Hit))
	{
		AActor* HitActor = Hit.GetActor();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DefaultImpactEffect, Hit.ImpactPoint);
	}
}
