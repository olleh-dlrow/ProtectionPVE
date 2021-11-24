// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PWeapon.h"
#include "PGrenadeLauncher.generated.h"

class APBullet;
/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API APGrenadeLauncher : public APWeapon
{
	GENERATED_BODY()

protected:
	virtual void NativeShoot(FAimHitInfo Info) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<APBullet> BulletClass;
};
