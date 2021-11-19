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

	UMainSceneWidget* MainScene = CreateWidget<UMainSceneWidget>(GetWorld(), MainSceneWidgetClass);
	APCharacter* MyCharacter = Cast<APCharacter>(GetCharacter());
	
	if(MainScene)
	{
		// 添加Character对Widget的引用
		MyCharacter->SetMainSceneWidget(MainScene);
		
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
		if(MyCharacter)
		{
			// 开局默认创建1把武器
			MyCharacter->CreateWeapon(0, MyCharacter->RifleWeaponClass, MyCharacter->RifleAttachSocketName);
			MyCharacter->SetMaxBulletCount(0, MyCharacter->GetMaxBulletCount(0));
			MyCharacter->SetRemainBulletCount(0, MyCharacter->GetMaxBulletCount(0));

			// 第2把武器
			MyCharacter->CreateWeapon(1, MyCharacter->GrenadeLauncherClass, MyCharacter->GrenadeLauncherSocketName);
			MyCharacter->SetMaxBulletCount(1, MyCharacter->GetMaxBulletCount(1));
			MyCharacter->SetRemainBulletCount(1, MyCharacter->GetMaxBulletCount(1));
	
		}
		MainScene->AddToViewport();

		if(TouchInterface)
			ActivateTouchInterface(TouchInterface);
	}
}
