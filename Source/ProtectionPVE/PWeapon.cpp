// Fill out your copyright notice in the Description page of Project Settings.


#include "PWeapon.h"

#include "PCore.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APWeapon::APWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	// MagazineMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MagazineMeshComp"));
	
	RootComponent = MeshComp;
	// MagazineMeshComp->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, MagazineSocketName);

	bCanSee = true;
	SetReplicates(true);
}

void APWeapon::Shoot(FAimHitInfo Info)
{
	// PCore::PrintOnScreen(GetWorld(), "Shoot", 2.f);
	NativeShoot(Info);
}

void APWeapon::NativeShoot(FAimHitInfo Info)
{
}

// Called when the game starts or when spawned
void APWeapon::BeginPlay()
{
	Super::BeginPlay();

	RemainBulletCount = MaxBulletCount;
	// PC = GetNetOwningPlayer()->GetPlayerController(GetWorld());
}

bool APWeapon::CheckAimHit(FHitResult& Hit) const
{
	return false;
	// if(!PC)
	// {
	// 	UE_LOG(LogActor, Warning, TEXT("CheckAimHit PlayerController is null"))	
	// }
	//
	// int32 x, y;
	// PC->GetViewportSize(x, y);

	// FVector WorldLocation, WorldDirection;
	// if(PC->DeprojectScreenPositionToWorld(x * 0.5f, y * 0.5f, WorldLocation, WorldDirection))
	// {
	// 	WorldDirection *= 10000.0f;
	//
	// 	FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();
	//
	// 	FCollisionQueryParams QueryParams;
	// 	QueryParams.AddIgnoredActor(this);
	// 	// QueryParams.AddIgnoredActor(CurrentWeapon);
	// 	QueryParams.bTraceComplex = true;
	// 	QueryParams.bReturnPhysicalMaterial = true;
	// 	
	// 	if(GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, CameraLocation + WorldDirection, ECC_Visibility, QueryParams))
	// 	{
	// 		return true;
	// 	}
	// }
	// return false;
}

// Called every frame
void APWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 未知的Bug，在其它地方设置visibility，但被服务端复制后失效，因此强制每帧更新
	MeshComp->SetVisibility(bCanSee);
}

void APWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APWeapon, RemainBulletCount);	
	DOREPLIFETIME(APWeapon, MaxBulletCount);
	DOREPLIFETIME(APWeapon, bCanSee);	
}

