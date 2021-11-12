// Fill out your copyright notice in the Description page of Project Settings.


#include "PCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PWeapon.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "PGrenade.h"
#include "PPlayerController.h"
#include "Camera/CameraActor.h"

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

	// ChildActorComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActorComp"));
	// ChildActorComp->SetupAttachment(RootComponent);
	// ChildActorComp->SetChildActorClass(ACameraActor::StaticClass());
	
	// 设置最大行走速度
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	DesiredMaxSpeed = MaxWalkSpeed;
}

// Called when the game starts or when spawned
void APCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 初始化复位相机
	// BackCameraChildActorComp->GetChildActor()->SetActorLocation(CameraComp->GetComponentLocation());
	// BackCameraChildActorComp->GetChildActor()->SetActorRotation(CameraComp->GetComponentQuat());
	
	UAnimInstance* AI = GetMesh()->GetAnimInstance();
	AI->OnMontageEnded.AddDynamic(this, &APCharacter::OnMontageEnded);
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
	LastTouchLocation = Location;
}

void APCharacter::DoTouchRepeat(ETouchIndex::Type FingerIndex, FVector Location)
{
	const FVector DeltaLocation = Location - LastTouchLocation;
	LastTouchLocation = Location;

	float DeltaPitch = DeltaLocation.Y * TurnSpeed * GetWorld()->GetDeltaSeconds();
	float DeltaYaw = DeltaLocation.X * TurnSpeed * GetWorld()->GetDeltaSeconds();
	AddControllerPitchInput(DeltaPitch);
	AddControllerYawInput(DeltaYaw);

	// 更新Yaw角
	CurrentYaw += DeltaYaw * YawRotateSpeed;
}

// Called every frame
void APCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bCanSprint)
	{
		SprintTick();
	}

	// 设置IKWeight
	if(CurrentWeapon && CurrentWeapon->MeshComp->IsVisible() && !bIsReloading && !bIsThrowing)
	{
		HandIKWeight = 1;
	}
	else
	{
		HandIKWeight = 0;
	}
}

void APCharacter::SprintTick()
{
	float Speed = GetCharacterMovement()->MaxWalkSpeed;

	int Dir = FMath::Sign(DesiredMaxSpeed - Speed);
	if (Dir == 1)
	{
		Speed = FMath::Min(Speed + Acceleration, DesiredMaxSpeed);
		GetCharacterMovement()->MaxWalkSpeed = Speed;
	}
	else if (Dir == -1)
	{
		Speed = FMath::Max(Speed - Acceleration, DesiredMaxSpeed);
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
	if(!CurrentWeapon)return;

	if(CurrentWeapon->CurrentBulletCount <= 0)
	{
		// 播放空弹音效
		
		return;
	}
	
	UAnimInstance* AI = GetMesh()->GetAnimInstance();
	if(bIsReloading || bIsThrowing || AI->Montage_IsPlaying(FireMontage))return;

	bIsFiring = true;
	// 子弹减少
	SetCurrentBulletCount(CurrentWeapon->CurrentBulletCount - 1);
	
	// 播放Montage 动画
	if(FireMontage && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(FireMontage))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Fire"));
		PlayAnimMontage(FireMontage);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, CurrentWeapon->GetActorLocation());
	}

	FHitResult Hit;
	if(CheckAimHit(Hit))
	{
		AActor* HitActor = Hit.GetActor();
		// GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue, HitActor->GetFName().ToString());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CurrentWeapon->DefaultImpactEffect, Hit.ImpactPoint);
			
		OnHit(Hit);
	}
}

bool APCharacter::CheckAimHit(FHitResult& Hit)
{
	const APlayerController* PC = GetController<APlayerController>();

	int32 x, y;
	PC->GetViewportSize(x, y);

	FVector WorldLocation, WorldDirection;
	if(PC->DeprojectScreenPositionToWorld(x * 0.5f, y * 0.5f, WorldLocation, WorldDirection))
	{
		WorldDirection *= 10000.0f;

		FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		// QueryParams.AddIgnoredActor(CurrentWeapon);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;
		
		if(GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, CameraLocation + WorldDirection, ECC_Visibility, QueryParams))
		{
			return true;
		}
	}
	return false;
}

void APCharacter::Throw()
{
	UAnimInstance* AI = GetMesh()->GetAnimInstance();
	bool bIsInAir = GetMovementComponent()->IsFalling();
	check(ThrowMontage);
	if(bIsReloading || bIsFiring || bIsInAir || AI->Montage_IsPlaying(ThrowMontage))return;

	bIsThrowing = true;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	Grenade = GetWorld()->SpawnActor<APGrenade>(GrenadeClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if(Grenade)
	{
		Grenade->SetOwner(this);
		Grenade->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ThrowAttachSocketName);
	}
	
	if(!AI->Montage_IsPlaying(ThrowMontage))
	{
		PlayAnimMontage(ThrowMontage);
		if(CurrentWeapon)
		{
			CurrentWeapon->MeshComp->SetVisibility(false);
		}
	}
}

void APCharacter::NotifyThrowOut()
{
	if(Grenade)
	{
		
		FHitResult Hit;
		if(CheckAimHit(Hit))
		{
			// 计算速度
			float Y1 = Grenade->GetActorLocation().Z;
			float Y2 = Hit.Location.Z;

			float Dist = FVector::Dist(Grenade->GetActorLocation(), Hit.Location);
			float DeltaY = Y2 - Y1;
			float DeltaX = FMath::Sqrt(Dist * Dist - DeltaY * DeltaY);

			float Vx = MaxThrowVelocity.X;
			float DeltaT = DeltaX / Vx;

			float Vy = (DeltaY - 0.5 * GetWorld()->GetGravityZ() * DeltaT * DeltaT) / DeltaT;
			Vy = FMath::Min(Vy, MaxThrowVelocity.Y);
			
			Grenade->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			Grenade->CapsuleComp->SetSimulatePhysics(true);
			FVector V = Hit.Location - Grenade->GetActorLocation();
			V.Z = 0;
			V.Normalize();
			V *= Vx;
			
			Grenade->CapsuleComp->SetPhysicsLinearVelocity(FVector(V.X, V.Y, Vy));
			// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue,  "DeltaY " + FString::SanitizeFloat(DeltaY));
			// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue,  "DeltaX " + FString::SanitizeFloat(DeltaX));
			// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, "Vy1 " + FString::SanitizeFloat(Vy1));			
		}
		
	}
}

void APCharacter::EnterFreeView()
{
	CurrentYaw = 0.0f;
	bInFreeView = true;
	bUseControllerRotationYaw = false;
	APlayerController* PC = GetController<APlayerController>();
	
	if(PC)
	{
		if(!FreeViewCamera)
		{
			FreeViewCamera = Cast<ACameraActor>(GetWorld()->SpawnActor(ACameraActor::StaticClass()));
		}
		if(FreeViewCamera)
		{
			FreeViewCamera->SetActorLocation(GetActorLocation() + FVCameraOffset);
			FreeViewCamera->SetActorRotation(FVCameraRotator);
		}
		
		PC->ActivateTouchInterface(nullptr);
		PC->SetViewTargetWithBlend(FreeViewCamera, 1.0f);
	}
}

void APCharacter::ExitFreeView()
{
	bInFreeView = false;
	bUseControllerRotationYaw = true;
	APPlayerController* PC = GetController<APPlayerController>();
	if(PC)
	{
		PC->ActivateTouchInterface(PC->TouchInterface);
		PC->SetViewTargetWithBlend(this, 1.0f);
	}
}

void APCharacter::SetCurrentBulletCount(int Count)
{
	CurrentWeapon->CurrentBulletCount = Count;
	CurrentBulletText->SetText(FText::AsNumber(Count));
}

void APCharacter::CreateWeapon(int Slot, TSubclassOf<APWeapon> WeaponClass, FName SocketName)
{
	APWeapon** WeaponPtr = nullptr;
	if(Slot == 1)
	{
		WeaponPtr = &Weapon1;
	}
	else if(Slot == 2)
	{
		WeaponPtr = &Weapon2;
	}
	else
	{
		return;
	}
	// attach rifle
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 初始化武器
	*WeaponPtr = GetWorld()->SpawnActor<APWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	APWeapon* Weapon = *WeaponPtr;
	if(Weapon)
	{
		Weapon->SetOwner(this);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		Weapon->MeshComp->SetVisibility(false);

		Weapon->CurrentBulletCount = Weapon->MaxBulletCount;
	}
}

void APCharacter::Reload()
{
	if(!CurrentWeapon)return;
	UAnimInstance* AI = GetMesh()->GetAnimInstance();
	bool bIsInAir = GetMovementComponent()->IsFalling();
	check(ReloadMontage);
	if(bIsFiring || bIsInAir || AI->Montage_IsPlaying(ReloadMontage))return;

	bIsReloading = true;

	if(!AI->Montage_IsPlaying(ReloadMontage))
	{
		PlayAnimMontage(ReloadMontage);
	}
}

void APCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupt)
{
	if(Montage == ThrowMontage)
	{
		bIsThrowing = false;
		if(CurrentWeapon)
		{
			CurrentWeapon->MeshComp->SetVisibility(true);
		}
	}
	else if(Montage == FireMontage)
	{
		bIsFiring = false;
	}
	else if(Montage == ReloadMontage)
	{
		bIsReloading = false;
		SetCurrentBulletCount(CurrentWeapon->MaxBulletCount);
	}
}



