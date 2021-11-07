// Fill out your copyright notice in the Description page of Project Settings.


#include "InputPlayerNameWidget.h"

#include "Components/EditableTextBox.h"
#include "PGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UInputPlayerNameWidget::OnButtonOKClicked(UEditableTextBox* PlayerNameTB)
{
	UPGameInstance* GI = Cast<UPGameInstance>(GetGameInstance());
	if(GI)
	{
		GI->PlayerName = PlayerNameTB->GetText().ToString();
	}
	RemoveFromParent();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainLevel"));
}
