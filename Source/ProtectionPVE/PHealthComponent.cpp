// Fill out your copyright notice in the Description page of Project Settings.


#include "PHealthComponent.h"

#include "PCharacter.h"
#include "PGameStateBase.h"
#include "PPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPHealthComponent::UPHealthComponent()
{
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UPHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if(GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if(MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UPHealthComponent::HandleTakeAnyDamage);
		}
	}
	
	Health = DefaultHealth;
}

void UPHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.f, DefaultHealth);
	
	APCharacter* MyCharacter = Cast<APCharacter>(GetOwner());
	if(MyCharacter)
	{
		
		if(Health <= 0.f && !MyCharacter->bDied)
		{
			MyCharacter->bDied = true;
			// 结算死亡数和击杀数
			APPlayerState* MyPS = MyCharacter->GetPlayerState<APPlayerState>();
			if(MyPS)
			{
				MyPS->SetDeathCount(MyPS->GetDeathCount() + 1);
				APGameStateBase* GS = Cast<APGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
				if(GS)
				{
					GS->SetTotalDeathCount(GS->GetTotalDeathCount() - 1);
				}
			}
	
			// 开启复活倒计时
			MyCharacter->SetReviveTime(MyCharacter->GetDefaultReviveTime());
			GetWorld()->GetTimerManager().SetTimer(MyCharacter->ReviveTimerHandle, MyCharacter, &APCharacter::ReviveCountDown, 1.f, true);
			
			MyCharacter->OnDied();
		}		
	}
}

void UPHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(UPHealthComponent, Health);
}




