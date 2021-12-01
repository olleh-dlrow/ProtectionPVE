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

void APPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	APCharacter* MyCharacter = Cast<APCharacter>(InPawn);
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

void APPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UMainSceneWidget* MainScene = nullptr;
	APCharacter* MyCharacter = GetPawn<APCharacter>();

	APPlayerState* PS = GetPlayerState<APPlayerState>();
	
	if(GetNetMode() == NM_Standalone || GetNetMode() == NM_Client || GetNetMode() == NM_ListenServer)
	{
		MainScene = CreateWidget<UMainSceneWidget>(GetWorld(), MainSceneWidgetClass);
	}
	if(MainScene)
	{
		// 添加Character对Widget的引用
		MyCharacter->SetMainSceneWidget(MainScene);
		
		MainScene->AddToViewport();
		
		if(TouchInterface && !MyCharacter->bPlayInPC)
			ActivateTouchInterface(TouchInterface);
	}
}

void APPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}


