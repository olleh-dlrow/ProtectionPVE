// Fill out your copyright notice in the Description page of Project Settings.


#include "PGameStateBase.h"

void APGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	RemainTime = MaxTime;
}

void APGameStateBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(RemainTime > 0 && !bIsPaused)
	{
		RemainTime -= DeltaSeconds;
	}
}
