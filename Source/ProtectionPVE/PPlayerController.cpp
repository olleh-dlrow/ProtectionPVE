// Fill out your copyright notice in the Description page of Project Settings.


#include "PPlayerController.h"

#include "PCharacter.h"
#include "PGameInstance.h"
#include "PGameStateBase.h"
#include "PPlayerState.h"
#include "PWeapon.h"
#include "Components/TextBlock.h"
#include "GameFramework/TouchInterface.h"
#include "Kismet/GameplayStatics.h"

void APPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* MainScene = CreateWidget<UMainSceneWidget>(GetWorld(), MainSceneWidgetClass);
	
	if(MainScene)
	{
		// 进入游戏时设置名称
		UTextBlock* NameText = Cast<UTextBlock>(MainScene->GetWidgetFromName(TEXT("NameText")));
		if(NameText)
		{
			UPGameInstance* GI = Cast<UPGameInstance>(GetGameInstance());
			if(GI)
			{
				NameText->SetText(FText::FromString(GI->PlayerName));
			}
		}
	
		// 添加GameState对Time的引用
		APGameStateBase* GameState = Cast<APGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
		if(GameState)
		{
			UTextBlock* TimeText = Cast<UTextBlock>(MainScene->GetWidgetFromName(TEXT("TimeText")));
			GameState->SetTimeText(TimeText);
			GameState->InitTimeText();
		}
	
		// 添加PlayerState对Score的引用
		APPlayerState* PS = GetPlayerState<APPlayerState>();
		if(PS)
		{
			UTextBlock* ScoreText = Cast<UTextBlock>(MainScene->GetWidgetFromName(TEXT("ScoreText")));
			PS->SetScoreText(ScoreText);
		}
	
		// 添加Character对子弹数目的引用
		APCharacter* MyCharacter = Cast<APCharacter>(GetCharacter());
		if(MyCharacter)
		{
			UTextBlock* CurrentBullet1Text = Cast<UTextBlock>(MainScene->GetWidgetFromName(TEXT("CurrentBullet1")));
			UTextBlock* CurrentBullet2Text = Cast<UTextBlock>(MainScene->GetWidgetFromName(TEXT("CurrentBullet2")));
			UTextBlock* MaxBullet1Text = Cast<UTextBlock>(MainScene->GetWidgetFromName(TEXT("MaxBullet1")));
			UTextBlock* MaxBullet2Text = Cast<UTextBlock>(MainScene->GetWidgetFromName(TEXT("MaxBullet2")));
	
			MyCharacter->CurrentBullet1Text = CurrentBullet1Text;
			MyCharacter->CurrentBullet2Text = CurrentBullet2Text;
			MyCharacter->MaxBullet1Text = MaxBullet1Text;
			MyCharacter->MaxBullet2Text = MaxBullet2Text;
			
			MyCharacter->CreateWeapon(1, MyCharacter->RifleWeaponClass, MyCharacter->RifleAttachSocketName);
			MyCharacter->CurrentBullet1Text->SetText(FText::AsNumber(MyCharacter->Weapon1->CurrentBulletCount));
			MyCharacter->MaxBullet1Text->SetText(FText::FromString("/" + FString::FromInt(MyCharacter->Weapon1->MaxBulletCount)));
			
			// if(MyCharacter->Weapon2)
			// {
			// 	MyCharacter->CurrentBullet2Text->SetText(FText::AsNumber(MyCharacter->Weapon2->CurrentBulletCount));
			// 	MyCharacter->MaxBullet2Text->SetText(FText::AsNumber(MyCharacter->Weapon2->MaxBulletCount));
			// }
	
		}
		MainScene->AddToViewport();

		if(TouchInterface)
			ActivateTouchInterface(TouchInterface);
	}
}
