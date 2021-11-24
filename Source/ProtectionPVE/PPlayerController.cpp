// Fill out your copyright notice in the Description page of Project Settings.


#include "PPlayerController.h"

#include "PCharacter.h"
#include "PGameInstance.h"
#include "PGameStateBase.h"
#include "PPlayerState.h"
#include "Components/TextBlock.h"
#include "GameFramework/TouchInterface.h"
#include "Kismet/GameplayStatics.h"
#include "PCore.h"

void APPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UMainSceneWidget* MainScene = nullptr;
	APCharacter* MyCharacter = GetPawn<APCharacter>();

	APPlayerState* PS = GetPlayerState<APPlayerState>();
	
	Cast<APCharacter>(GetOwner());
	
	if(GetLocalRole() < ROLE_Authority)
	{
		MainScene = CreateWidget<UMainSceneWidget>(GetWorld(), MainSceneWidgetClass);
	}
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
		if(PS)
		{
			UTextBlock* ScoreText = Cast<UTextBlock>(MainScene->GetWidgetFromName(TEXT("ScoreText")));
			PS->SetScoreText(ScoreText);
		}
		
		MainScene->AddToViewport();
		
		if(TouchInterface && !MyCharacter->bPlayInPC)
			ActivateTouchInterface(TouchInterface);
	}

	// 添加Character对子弹数目的引用
	// if(MyCharacter)
	// {
	// 	// 开局默认创建1把武器
	// 	MyCharacter->CreateWeapon(0, MyCharacter->RifleWeaponClass, MyCharacter->RifleAttachSocketName);
	// 	MyCharacter->SetMaxBulletCount(0, MyCharacter->GetMaxBulletCount(0));
	// 	MyCharacter->SetRemainBulletCount(0, MyCharacter->GetMaxBulletCount(0));
	//
	// 	// 第2把武器
	// 	MyCharacter->CreateWeapon(1, MyCharacter->GrenadeLauncherClass, MyCharacter->GrenadeLauncherSocketName);
	// 	MyCharacter->SetMaxBulletCount(1, MyCharacter->GetMaxBulletCount(1));
	// 	MyCharacter->SetRemainBulletCount(1, MyCharacter->GetMaxBulletCount(1));
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Character is null"))
	// }

	// if(PS)
	// {
	// 	PCore::PrintOnScreen(this, "PS Success", 2.f);
	// 	PS->CreateWeapon(0, PS->GetWeaponClass("Rifle"), PS->GetWeaponAttachSocketName("Rifle"));
	//
	// 	// 第2把武器
	// 	PS->CreateWeapon(1, PS->GetWeaponClass("GrenadeLauncher"), PS->GetWeaponAttachSocketName("GrenadeLauncher"));
	// }
}

void APPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(!bPossessed && GetPawn())
	{
		// PCore::PrintOnScreen(this, "GetPawnSuccess", 2.f);
		bPossessed = true;
		APCharacter* MyCharacter = GetPawn<APCharacter>();
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
	}
}

void APPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(APPlayerController, bPossessed);
}

