// Fill out your copyright notice in the Description page of Project Settings.


#include "PRifle.h"


#include "PCore.h"
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
			// PCore::PrintOnScreen(GetWorld(), Hit.GetActor()->GetName() + FString(" Take Damage"), 2.f);
			UGameplayStatics::ApplyPointDamage(Hit.GetActor(),
				GetDamage(),
				Info.TraceTo - Info.TraceFrom,
				Hit,
				GetOwner()->GetInstigatorController(),
				GetOwner(),
				DamageTypeClass);
			
			if(DefaultImpactEffect)
				SpawnEffect(Info.HitLocation);
		}
	}
	
	// FHitResult Hit;
	// if(CheckAimHit(Hit))
	// {
	// 	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DefaultImpactEffect, Hit.ImpactPoint);
	// }		
}

void APRifle::SpawnEffect_Implementation(FVector ImpactPoint)
{
	if(GetLocalRole() < ROLE_Authority)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DefaultImpactEffect, ImpactPoint);
}
