// Fill out your copyright notice in the Description page of Project Settings.


#include "PGrenade.h"

#include "PCore.h"

// Sets default values
APGrenade::APGrenade()
{
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	
	RootComponent = CapsuleComp;
	MeshComp->SetupAttachment(CapsuleComp);

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void APGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}


