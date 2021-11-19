// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PWeapon.h"
#include "PRifle.generated.h"

/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API APRifle : public APWeapon
{
	GENERATED_BODY()
protected:
	virtual void NativeShoot() override;
};
