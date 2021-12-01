// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "PGrenade.generated.h"

UCLASS()
class PROTECTIONPVE_API APGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APGrenade();

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="PGrenade")
	float Damage = 40.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="PGrenade")
	float DamageRadius = 300.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PGrenade")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PGrenade")
	UCapsuleComponent* CapsuleComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
