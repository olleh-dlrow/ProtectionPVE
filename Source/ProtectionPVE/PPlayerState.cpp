// Fill out your copyright notice in the Description page of Project Settings.


#include "PPlayerState.h"

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

void APPlayerState::BeginPlay()
{
	Super::BeginPlay();
}
