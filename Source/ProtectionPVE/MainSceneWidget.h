// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MainSceneWidget.generated.h"

class APCharacter;
class APWeapon;

/**
 * 
 */
UCLASS()
class PROTECTIONPVE_API UMainSceneWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category="WidgetClass")
	TSubclassOf<UUserWidget> PauseWidgetClass;
	
	UPROPERTY(VisibleAnywhere, Category="Ref")
	APCharacter* Character;

	// 各种控件引用
	UPROPERTY()
	TArray<UTextBlock*> RemainBulletTexts = {nullptr, nullptr};

	UPROPERTY()
	TArray<UTextBlock*> MaxBulletTexts = {nullptr, nullptr};

	UPROPERTY()
	class UButton* PickupBtn;
public:
	// 控件接口
	void SetRemainBulletText(int Index, const FString& Text);

	void SetMaxBulletText(int Index, const FString& Text);

	void SetPickupButtonVisibility(float Visible);
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void OnPauseButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnFireButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnJumpButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnCrouchButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnThrowButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnWeaponSlot1Clicked();

	UFUNCTION(BlueprintCallable)
	void OnWeaponSlot2Clicked();

	UFUNCTION(BlueprintCallable)
	void OnReloadButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnFreeViewButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnPickupButtonClicked();
};
