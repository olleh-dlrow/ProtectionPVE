// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"

/**
 * 
 */
class PROTECTIONPVE_API PCore
{
public:
	PCore();
	~PCore();

	static void PrintOnScreen(UObject* WorldContextObject, const FString& InString, float Duration);
};
