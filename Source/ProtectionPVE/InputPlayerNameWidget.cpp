// Fill out your copyright notice in the Description page of Project Settings.


#include "InputPlayerNameWidget.h"

#include "Components/EditableTextBox.h"
#include "PGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UInputPlayerNameWidget::OnButtonOKClicked()
{
	UPGameInstance* GI = Cast<UPGameInstance>(GetGameInstance());
	if(GI)
	{
		UEditableTextBox* NameETB = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("NameETB")));
		GI->PlayerName = NameETB->GetText().ToString();
	}
	RemoveFromParent();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainLevel"));
}
