// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTECTIONPVE_API UPHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPHealthComponent();

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="PHealth")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PHealth")
	float DefaultHealth = 100.f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	float GetHealth() const {return Health;}

	UFUNCTION()
	void SetHealth(float Value) {Health = Value;}
	
	UFUNCTION()
	float GetDefaultHealth() const {return DefaultHealth;}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
