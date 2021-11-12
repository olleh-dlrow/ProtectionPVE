// Fill out your copyright notice in the Description page of Project Settings.


#include "PGrenade.h"

// Sets default values
APGrenade::APGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	
	RootComponent = CapsuleComp;
	MeshComp->SetupAttachment(CapsuleComp);
}

// Called when the game starts or when spawned
void APGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

