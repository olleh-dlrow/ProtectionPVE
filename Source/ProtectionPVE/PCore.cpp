// Fill out your copyright notice in the Description page of Project Settings.


#include "PCore.h"


PCore::PCore()
{
}

PCore::~PCore()
{
}

void PCore::PrintOnScreen(UObject* WorldContextObject, const FString& InString, float Duration)
{
	UKismetSystemLibrary::PrintString(WorldContextObject, InString, true, true, FColor::Blue, Duration);
}
