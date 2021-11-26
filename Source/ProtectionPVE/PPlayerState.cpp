// Fill out your copyright notice in the Description page of Project Settings.


#include "PPlayerState.h"

#include "PCharacter.h"
#include "Components/TextBlock.h"

void APPlayerState::SetPlayerScore(float NewScore)
{
	SetScore(NewScore);

	if(ScoreText)
	{
		ScoreText->SetText(FText::AsNumber(NewScore));
	}
}

void APPlayerState::SetScoreText(UTextBlock* Text)
{
	ScoreText = Text;
}

void APPlayerState::CreateWeapon(int Slot, TSubclassOf<APWeapon> WeaponClass, FName SocketName)
{
	APCharacter* MyCharacter = GetPawn<APCharacter>();
	if(MyCharacter)
	{
		MyCharacter->CreateWeapon(Slot, WeaponClass, SocketName);
	}
}

TSubclassOf<APWeapon> APPlayerState::GetWeaponClass(const FString& WeaponName)
{
	APCharacter* MyCharacter = GetPawn<APCharacter>();
	if(MyCharacter)
	{
		if(WeaponName == "Rifle")
		{
			return MyCharacter->RifleWeaponClass;
		}
		else if(WeaponName == "GrenadeLauncher")
		{
			return MyCharacter->GrenadeLauncherClass;
		}
	}
	return nullptr;
}

FName APPlayerState::GetWeaponAttachSocketName(const FString& WeaponName)
{
	APCharacter* MyCharacter = GetPawn<APCharacter>();
	if(MyCharacter)
	{
		if(WeaponName == "Rifle")
		{
			return MyCharacter->RifleAttachSocketName;
		}
		else if(WeaponName == "GrenadeLauncher")
		{
			return MyCharacter->GrenadeLauncherSocketName;
		}
	}
	return "";
}

void APPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void APPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(APPlayerState, KillCount);
	DOREPLIFETIME(APPlayerState, DeathCount);
}
