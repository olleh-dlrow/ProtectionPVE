// Fill out your copyright notice in the Description page of Project Settings.


#include "PCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PWeapon.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
APCharacter::APCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	// 相机跟着角色旋转
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// 设置最大行走速度
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	DesiredMaxSpeed = MaxWalkSpeed;

	RifleAttachSocketName = "RifleSocket";
}

// Called when the game starts or when spawned
void APCharacter::BeginPlay()
{
	Super::BeginPlay();

	// attach rifle
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentWeapon = GetWorld()->SpawnActor<APWeapon>(RifleWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if(CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RifleAttachSocketName);
	}
}

void APCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void APCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void APCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APCharacter::DoTouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
	TouchStart = Location;
}

void APCharacter::DoTouchRepeat(ETouchIndex::Type FingerIndex, FVector Location)
{
	const FVector DeltaLocation = Location - TouchStart;
	TouchStart = Location;
	AddControllerPitchInput(DeltaLocation.Y * TurnSpeed * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(DeltaLocation.X * TurnSpeed * GetWorld()->GetDeltaSeconds());
}


// Called every frame
void APCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bCanSprint)
	{
		SprintTick();
	}

}

void APCharacter::SprintTick()
{
	float Speed = GetCharacterMovement()->MaxWalkSpeed;

	int Dir = FMath::Sign(DesiredMaxSpeed - Speed);
	if (Dir == 1)
	{
		Speed = FMath::Min(Speed + Accelaration, DesiredMaxSpeed);
		GetCharacterMovement()->MaxWalkSpeed = Speed;
	}
	else if (Dir == -1)
	{
		Speed = FMath::Max(Speed - Accelaration, DesiredMaxSpeed);
		GetCharacterMovement()->MaxWalkSpeed = Speed;
	}
}

// Called to bind functionality to input
void APCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APCharacter::MoveRight);
	
#if !PLATFORM_ANDROID
	if(bOpenMouseTurn)
	{
		PlayerInputComponent->BindAxis("LookUp", this, &APCharacter::AddControllerPitchInput);
		PlayerInputComponent->BindAxis("Turn", this, &APCharacter::AddControllerYawInput);	
	}
#endif
	
	PlayerInputComponent->BindAxis("TurnRate", this, &APCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &APCharacter::ChangeWalkToSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &APCharacter::ChangeSprintToWalk);

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &APCharacter::Fire);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &APCharacter::DoTouchPressed);

	PlayerInputComponent->BindTouch(IE_Repeat, this, &APCharacter::DoTouchRepeat);
}

void APCharacter::ChangeWalkToSprint()
{
	DesiredMaxSpeed = MaxSprintSpeed;
}

void APCharacter::ChangeSprintToWalk()
{
	DesiredMaxSpeed = MaxWalkSpeed;
}

void APCharacter::Fire()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	int32 x, y;
	PC->GetViewportSize(x, y);

	FVector WorldLocation, WorldDirection;
	if(PC->DeprojectScreenPositionToWorld(x * 0.5f, y * 0.5f, WorldLocation, WorldDirection))
	{
		WorldDirection *= 10000.0f;

		FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(CurrentWeapon);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		FHitResult Hit;
		if(GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, CameraLocation + WorldDirection, ECC_Visibility, QueryParams))
		{
			AActor* HitActor = Hit.GetActor();
			// GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue, HitActor->GetFName().ToString());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CurrentWeapon->DefaultImpactEffect, Hit.ImpactPoint);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, Hit.Location);
			
			OnHit(Hit);
		}

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString(TEXT("can't convert")));
	}

}

