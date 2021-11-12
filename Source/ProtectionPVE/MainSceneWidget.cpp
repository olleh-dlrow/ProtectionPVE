// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSceneWidget.h"

#include "PCharacter.h"
#include "PGameInstance.h"
#include "PGameStateBase.h"
#include "PPlayerController.h"
#include "Components/TextBlock.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PWeapon.h"


void UMainSceneWidget::OnWidgetConstructed()
{
	Character = Cast<APCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UPGameInstance* GI = Cast<UPGameInstance>(GetGameInstance());
	check(GI);
	
	// 设置玩家名称
	UTextBlock* NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));
	NameText->SetText(FText::FromString(GI->PlayerName));
}

void UMainSceneWidget::OnPauseButtonClicked()
{
	Character->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	APGameStateBase* GameState = Cast<APGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if(GameState)
	{
		GameState->bIsPaused = true;
		UUserWidget* PauseWidget = CreateWidget(this, PauseWidgetClass);
		PauseWidget->AddToViewport();
	}
	
}

void UMainSceneWidget::OnFireButtonClicked()
{
	Character->Fire();
}

void UMainSceneWidget::OnJumpButtonClicked()
{
	Character->Jump();
}

void UMainSceneWidget::OnCrouchButtonClicked()
{
	if(!Character->GetMovementComponent()->IsCrouching())
	{
		Character->Crouch();
	}
	else
	{
		Character->UnCrouch();
	}
}

void UMainSceneWidget::OnThrowButtonClicked()
{
	Character->Throw();
}

void UMainSceneWidget::OnWeaponSlot1Clicked()
{
	// 没有武器
	if(!Character->Weapon1)return;

	// 该武器为当前武器，收回
	if(Character->Weapon1 == Character->CurrentWeapon)
	{
		Character->ShootWeight = 0;
		Character->CurrentWeapon->MeshComp->SetVisibility(false);
		Character->CurrentBulletText = nullptr;
		Character->CurrentWeapon = nullptr;
	}
	// 该武器不是当前武器
	else
	{
		// 没持有武器，则装备该武器
		if(!Character->CurrentWeapon)
		{
			Character->CurrentWeapon = Character->Weapon1;
			Character->CurrentBulletText = Character->CurrentBullet1Text;
			Character->CurrentWeapon->MeshComp->SetVisibility(true);
			Character->ShootWeight = 1;
		}
		// 持有武器，则换下当前武器再装备该武器
		else
		{
			Character->CurrentWeapon->MeshComp->SetVisibility(false);
			Character->CurrentWeapon = Character->Weapon1;
			Character->CurrentBulletText = Character->CurrentBullet1Text;
		}
	}
}

void UMainSceneWidget::OnReloadButtonClicked()
{
	Character->Reload();
}

void UMainSceneWidget::OnFreeViewButtonClicked()
{
	 if(Character->bInFreeView)
	 {
		Character->ExitFreeView();
	 }
	else
	{
		Character->EnterFreeView();
	}
}


