// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSceneWidget.h"

#include "PCharacter.h"
#include "PGameInstance.h"
#include "PGameStateBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UMainSceneWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// 设置武器栏控件
	RemainBulletTexts[0] = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentBullet1")));
	RemainBulletTexts[1] = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentBullet2")));
	MaxBulletTexts[0] = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxBullet1")));
	MaxBulletTexts[1] = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxBullet2")));

	PickupBtn = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Pickup")));
	PickupBtn->SetVisibility(ESlateVisibility::Hidden);
}

void UMainSceneWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 初始化外部引用
	// Character = Cast<APCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	// Character引用MainSceneWidget
	// Character->SetMainSceneWidget(this);
	
	// 获取GameInstance
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
	// Character->Fire();
}

void UMainSceneWidget::OnJumpButtonClicked()
{
	Character->Jump();
}

void UMainSceneWidget::OnCrouchButtonClicked()
{
	Character->UpdateCrouchState();
}

void UMainSceneWidget::OnThrowButtonClicked()
{
	Character->Throw();
}

void UMainSceneWidget::OnWeaponSlot1Clicked()
{
	Character->SwitchWeapon(0);
}

void UMainSceneWidget::OnWeaponSlot2Clicked()
{
	Character->SwitchWeapon(1);
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

void UMainSceneWidget::OnPickupButtonClicked()
{
	Character->PickupWeapon();
}

void UMainSceneWidget::SetRemainBulletText(int Index, const FString& Text)
{
	if(Index >= 0 && Index < RemainBulletTexts.Num())
	{
		RemainBulletTexts[Index]->SetText(FText::FromString(Text));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetRemainBulletText index out of range"))
	}
}

void UMainSceneWidget::SetMaxBulletText(int Index, const FString& Text)
{
	if(Index >= 0 && Index < MaxBulletTexts.Num())
	{
		MaxBulletTexts[Index]->SetText(FText::FromString((Text)));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetMaxBulletText index out of range"))
	}
}

void UMainSceneWidget::SetPickupButtonVisibility(float Visible)
{
	if(Visible)
	{
		PickupBtn->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PickupBtn->SetVisibility(ESlateVisibility::Hidden);
	}
}


