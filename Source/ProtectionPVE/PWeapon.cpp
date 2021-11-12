// Fill out your copyright notice in the Description page of Project Settings.


#include "PWeapon.h"

// Sets default values
APWeapon::APWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	// MagazineMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MagazineMeshComp"));
	
	RootComponent = MeshComp;
	// MagazineMeshComp->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, MagazineSocketName);
}

// Called when the game starts or when spawned
void APWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

