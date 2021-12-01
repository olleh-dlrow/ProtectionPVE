// Fill out your copyright notice in the Description page of Project Settings.


#include "PRifle.h"


#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void APRifle::NativeShoot(FAimHitInfo Info)
{
	Super::NativeShoot(Info);
	
	// 命中检测
	if(Info.bHitHappened)
	{
		// 服务器端判定伤害
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		FHitResult Hit;
		if(GetWorld()->LineTraceSingleByChannel(Hit, Info.TraceFrom, Info.TraceTo, ECC_Pawn, QueryParams))
		{
			if(Hit.Actor->ActorHasTag("Enemy"))
			{
				UGameplayStatics::ApplyPointDamage(Hit.GetActor(),
					GetDamage(),
					Info.TraceTo - Info.TraceFrom,
					Hit,
					GetOwner()->GetInstigatorController(),
					GetOwner(),
					DamageTypeClass);
			}

			
			if(DefaultImpactEffect)
				SpawnEffect(Info.HitLocation);
		}
	}
}

void APRifle::SpawnEffect_Implementation(FVector ImpactPoint)
{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DefaultImpactEffect, ImpactPoint);
}
