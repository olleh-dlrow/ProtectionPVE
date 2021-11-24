// Fill out your copyright notice in the Description page of Project Settings.


#include "PHealthComponent.h"

#include "PCharacter.h"
#include "PCore.h"

// Sets default values for this component's properties
UPHealthComponent::UPHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
	
	const FString Out = FString("Current Health: ") + FString::SanitizeFloat(Health);
	PCore::PrintOnScreen(GetWorld(), Out, 2.f);
	if(Health <= 0.f)
	{
		APCharacter* MyCharacter = Cast<APCharacter>(GetOwner());
		if(MyCharacter)
		{
			MyCharacter->bDied = true;
			MyCharacter->OnDied();
		}
	}
}

// Called every frame
void UPHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(UPHealthComponent, Health);
}




