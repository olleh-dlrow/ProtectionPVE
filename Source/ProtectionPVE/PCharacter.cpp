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
#include "MainSceneWidget.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "PCore.h"
#include "PHealthComponent.h"

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

	// 注册事件
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APCharacter::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APCharacter::OnEndOverlap);

	HealthComp = CreateDefaultSubobject<UPHealthComponent>(TEXT("HealthComp"));
}

// Called when the game starts or when spawned
void APCharacter::BeginPlay()
{
	Super::BeginPlay();

	UAnimInstance* AI = GetMesh()->GetAnimInstance();
	AI->OnMontageEnded.AddDynamic(this, &APCharacter::OnMontageEnded);
}

void APCharacter::MoveForward(float Value)
{
	if(!bDied)
		AddMovementInput(GetActorForwardVector() * Value);
}

void APCharacter::MoveRight(float Value)
{
	if(!bDied)
		AddMovementInput(GetActorRightVector() * Value);
}

void APCharacter::TurnBody(float Value)
{
	// PCore::PrintOnScreen(this, "TurnBody", 1.f);
	AddControllerYawInput(Value);
}

void APCharacter::LookUp(float Value)
{
	// PCore::PrintOnScreen(this, "LookUp", 1.f);
	AddControllerPitchInput(Value);
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

	// if(Weapons[0])
	// {
	// 	bool V = Weapons[0]->MeshComp->IsVisible();
	// 	PCore::PrintOnScreen(this, V ? "visible" : "not visible", 0.f);		
	// }

	// PCore::PrintOnScreen(this, FString::SanitizeFloat(RemoteViewPitch * 360.f / 255.f), 0.f);
	
	if(bCanSprint)
	{
		SprintTick();
	}

	// 设置IKWeight
	if(GetCurrentWeapon() && GetCurrentWeapon()->MeshComp->IsVisible() && !bIsReloading && !bIsThrowing)
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
	if(bPlayInPC)
	{
		PlayerInputComponent->BindAxis("LookUp", this, &APCharacter::LookUp);
		PlayerInputComponent->BindAxis("Turn", this, &APCharacter::TurnBody);	
	}
	else
	{
		PlayerInputComponent->BindTouch(IE_Pressed, this, &APCharacter::DoTouchPressed);
		PlayerInputComponent->BindTouch(IE_Repeat, this, &APCharacter::DoTouchRepeat);
	}
#endif
	
	//PlayerInputComponent->BindAxis("TurnRate", this, &APCharacter::TurnAtRate);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &APCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &APCharacter::ChangeWalkToSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &APCharacter::ChangeSprintToWalk);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APCharacter::Jump);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &APCharacter::StartFire);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APCharacter::UpdateCrouchState);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &APCharacter::StartThrow);
	PlayerInputComponent->BindAction("Weapon1", IE_Pressed, this, &APCharacter::SwitchWeapon0);
	PlayerInputComponent->BindAction("Weapon2", IE_Pressed, this, &APCharacter::SwitchWeapon1);
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &APCharacter::Server_PickupWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APCharacter::Server_Reload);
}

void APCharacter::ChangeWalkToSprint()
{
	DesiredMaxSpeed = MaxSprintSpeed;
}

void APCharacter::ChangeSprintToWalk()
{
	DesiredMaxSpeed = MaxWalkSpeed;
}

void APCharacter::StartFire()
{
	FHitResult Hit;
	const bool bHitHappened = CheckAimHit(Hit);
	
	FAimHitInfo Info;
	Info.bHitHappened = bHitHappened;
	Info.HitLocation = Hit.Location;
	CalculateTraceEnds(Info.TraceFrom, Info.TraceTo);
	if(GetCurrentWeapon())
	{
		Info.SpawnLocation = GetCurrentWeapon()->MeshComp->GetSocketLocation(GetCurrentWeapon()->MuzzleSocketName);
		Info.SpawnRotation = GetCurrentWeapon()->MeshComp->GetSocketRotation(GetCurrentWeapon()->MuzzleSocketName);
	}

	Server_Fire(Info);
}

void APCharacter::StartThrow()
{
	FHitResult Hit;
	const bool bHitHappened = CheckAimHit(Hit);
	
	FAimHitInfo Info;
	Info.bHitHappened = bHitHappened;
	Info.HitLocation = Hit.Location;
	
	Server_Throw(Info);
}

void APCharacter::Server_Fire_Implementation(FAimHitInfo Info)
{
	Fire(Info);
}

void APCharacter::Fire_Implementation(FAimHitInfo Info)
{
	// PCore::PrintOnScreen(this, "Fire", 2.f);
	
	if(!GetCurrentWeapon())return;

	if(GetCurrentRemainBulletCount() <= 0)
	{
		// 播放空弹音效
		
		return;
	}
	
	UAnimInstance* AI = GetMesh()->GetAnimInstance();
	if(bDied || bIsReloading || bIsThrowing || AI->Montage_IsPlaying(FireMontage))return;

	// 可以开火
	bIsFiring = true;
	// 子弹减少
	SetCurrentRemainBulletCount(GetCurrentRemainBulletCount() - 1);
	
	// 播放Montage 动画
	if(FireMontage && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(FireMontage))
	{
		PlayAnimMontage(FireMontage);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetCurrentWeapon()->GetActorLocation());
	}
	
	if(GetLocalRole() == ROLE_Authority)
		GetCurrentWeapon()->Shoot(Info);
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

		const FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;
		
		if(GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, CameraLocation + WorldDirection, ECC_Pawn, QueryParams))
		{
			// PCore::PrintOnScreen(GetWorld(), Hit.Actor->GetName(), 2.f);
			return true;
		}
	}
	return false;
}

void APCharacter::CalculateTraceEnds(FVector& TraceFrom, FVector& TraceTo)
{
	const APlayerController* PC = GetController<APlayerController>();

	int32 x, y;
	PC->GetViewportSize(x, y);
	FVector WorldLocation, WorldDirection;
	if(PC->DeprojectScreenPositionToWorld(x * 0.5f, y * 0.5f, WorldLocation, WorldDirection))
	{
		WorldDirection *= 10000.0f;

		const FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();

		TraceFrom = CameraLocation;
		TraceTo = CameraLocation + WorldDirection;
	}
}


bool APCharacter::CheckWeaponIndex(int Index) const
{
	return Index >= 0 && Index < Weapons.Num();
}

void APCharacter::Server_Throw_Implementation(FAimHitInfo Info)
{
	AimHitInfo = Info;
	
	Throw();
}

void APCharacter::Throw_Implementation()
{
	UAnimInstance* AI = GetMesh()->GetAnimInstance();
	bool bIsInAir = GetMovementComponent()->IsFalling();
	check(ThrowMontage);
	if(bDied || bIsReloading || bIsFiring || bIsInAir || AI->Montage_IsPlaying(ThrowMontage))return;

	bIsThrowing = true;
	
	if(!AI->Montage_IsPlaying(ThrowMontage))
	{
		// PCore::PrintOnScreen(this, "ThrowMontage", 2.f);
		PlayAnimMontage(ThrowMontage);
		if(GetCurrentWeapon())
		{
			// PCore::PrintOnScreen(this, "Can not see", 3.f);
			GetCurrentWeapon()->bCanSee = false;
		}
	}
	
	if(GetLocalRole() == ROLE_Authority)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		Grenade = GetWorld()->SpawnActor<APGrenade>(GrenadeClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if(Grenade)
		{
			Grenade->SetOwner(this);
			Grenade->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ThrowAttachSocketName);
		}
	}
}

void APCharacter::UpdateCrouchState()
{
	if(!GetMovementComponent()->IsCrouching())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void APCharacter::Server_Reload_Implementation()
{
	Reload();
}

void APCharacter::OnDied_Implementation()
{
	bUseControllerRotationYaw = false;
}

void APCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APWeapon* Weapon = Cast<APWeapon>(OtherActor);
	if(Weapon)
	{
		// 拾起武器
		// 显示widget，指示拾起
		if(GetMainSceneWidget())
			GetMainSceneWidget()->SetPickupButtonVisibility(true);
		DesiredPickupWeapon = Weapon;

		bCanPickup = true;
	}
}

void APCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APWeapon* Weapon = Cast<APWeapon>(OtherActor);
	if(Weapon)
	{
		if(GetMainSceneWidget())
			GetMainSceneWidget()->SetPickupButtonVisibility(false);

		bCanPickup = false;
	}
}


void APCharacter::NotifyThrowOut()
{
	// PCore::PrintOnScreen(this, "ThrowOut", 2.f);
	if(GetLocalRole() == ROLE_Authority)
	{
		if(!Grenade)return;
		
		if(AimHitInfo.bHitHappened)
		{
			const FVector HitLocation = AimHitInfo.HitLocation;
			float Y1 = Grenade->GetActorLocation().Z;
			float Y2 = HitLocation.Z;

			float Dist = FVector::Dist(Grenade->GetActorLocation(), HitLocation);
			float DeltaY = Y2 - Y1;
			float DeltaX = FMath::Sqrt(Dist * Dist - DeltaY * DeltaY);

			float Vx = MaxThrowVelocity.X;
			float DeltaT = DeltaX / Vx;

			float Vy = (DeltaY - 0.5 * GetWorld()->GetGravityZ() * DeltaT * DeltaT) / DeltaT;
			Vy = FMath::Min(Vy, MaxThrowVelocity.Y);
			
			Grenade->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			Grenade->CapsuleComp->SetSimulatePhysics(true);
			FVector V = HitLocation - Grenade->GetActorLocation();
			V.Z = 0;
			V.Normalize();
			V *= Vx;
			
			Grenade->CapsuleComp->SetPhysicsLinearVelocity(FVector(V.X, V.Y, Vy));			
		}
		else
		{
			const FVector HitLocation = AimHitInfo.HitLocation;
			float Vx = MaxThrowVelocity.X;
			float Vy = MaxThrowVelocity.Y;
			Grenade->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			Grenade->CapsuleComp->SetSimulatePhysics(true);
			FVector V = HitLocation - Grenade->GetActorLocation();
			V.Z = 0;
			V.Normalize();
			V *= Vx;
			
			Grenade->CapsuleComp->SetPhysicsLinearVelocity(FVector(V.X, V.Y, Vy));
		}
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue,  "DeltaY " + FString::SanitizeFloat(DeltaY));
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue,  "DeltaX " + FString::SanitizeFloat(DeltaX));
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, "Vy1 " + FString::SanitizeFloat(Vy1));			
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

int APCharacter::GetRemainBulletCount(int Index) const
{
	APWeapon* Weapon = GetWeapon(Index);
	if(Weapon)
	{
		return Weapon->RemainBulletCount;
	}
	
	// UE_LOG(LogActor, Warning, TEXT("Weapon is null"))
	return 0;
}

int APCharacter::GetMaxBulletCount(int Index) const
{
	APWeapon* Weapon = GetWeapon(Index);
	if(Weapon)return Weapon->MaxBulletCount;
	
	// UE_LOG(LogActor, Warning, TEXT("Weapon is null"))
	return 0;
}

APWeapon* APCharacter::GetWeapon(int Index) const
{
	if(CheckWeaponIndex(Index))
	{
		return Weapons[Index];
	}
	else
	{
		UE_LOG(LogActor, Warning, TEXT("GetWeapon index out of range"))
	}
	return nullptr;
}

APWeapon* APCharacter::GetCurrentWeapon() const
{
	if(CurrentWeaponIndex == -1)
		return nullptr;
	return GetWeapon(CurrentWeaponIndex);
}

void APCharacter::SetNewWeapon(int Index, APWeapon* NewWeapon)
{
	if(CheckWeaponIndex(Index))
	{
		Weapons[Index] = NewWeapon;
		NewWeapon->SetOwner(this);
		NewWeapon->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			GetWeaponAttachSocketName(NewWeapon->GetType()));
		
		SetMaxBulletCount(Index, NewWeapon->MaxBulletCount);
		SetRemainBulletCount(Index, NewWeapon->RemainBulletCount);
	}
	else
	{
		UE_LOG(LogActor, Warning, TEXT("SetWeapon index out of range"))
	}
}

void APCharacter::SetRemainBulletCount(int Index, int Count)
{
	APWeapon* Weapon = GetWeapon(Index);
	if(!Weapon)
	{
		UE_LOG(LogActor, Warning, TEXT("SetRemainBulletCount index out of range"))
	}
	else
	{
		Weapon->RemainBulletCount = Count;
		if(MainSceneWidget)
		{
			MainSceneWidget->SetRemainBulletText(Index, FString::FromInt(Count));
		}
	}
}

void APCharacter::SetMaxBulletCount(int Index, int Count)
{
	APWeapon* Weapon = GetWeapon(Index);
	if(!Weapon)
	{
		UE_LOG(LogActor, Warning, TEXT("SetMaxBulletCount index out of range"))
	}
	else
	{
		Weapon->MaxBulletCount = Count;
		if(MainSceneWidget)
		{
			MainSceneWidget->SetMaxBulletText(Index, "/" + FString::FromInt(Count));
		}
	}
}

void APCharacter::SetCurrentWeaponInSlot(int Slot)
{
	if(Slot >= 0 && Slot < Weapons.Num())
	{
		CurrentWeaponIndex = Slot;
		// PCore::PrintOnScreen(GetWorld(), FString::FromInt(CurrentWeaponIndex), 2.f);
	}
	else
		UE_LOG(LogActor, Warning, TEXT("SetCurrentWeapon index out of range"))
}

void APCharacter::PutBackCurrentWeapon()
{
	CurrentWeaponIndex = -1;
}

int APCharacter::GetCurrentRemainBulletCount() const
{
	return GetRemainBulletCount(CurrentWeaponIndex);
}

void APCharacter::SetCurrentRemainBulletCount(int Count)
{
	SetRemainBulletCount(CurrentWeaponIndex, Count);
}

int APCharacter::GetCurrentMaxBulletCount() const
{
	return GetMaxBulletCount(CurrentWeaponIndex);
}

void APCharacter::SetCurrentMaxBulletCount(int Count)
{
	SetMaxBulletCount(CurrentWeaponIndex, Count);
}

FName APCharacter::GetWeaponAttachSocketName(EWeapon Type)
{
	switch (Type)
	{
	case EWeapon::Rifle:
		return RifleAttachSocketName;
	case EWeapon::GrenadeLauncher:
		return GrenadeLauncherSocketName;
	default:
		UE_LOG(LogActor, Warning, TEXT("GetWeaponAttachSocketName not found"))
		return RifleAttachSocketName;		
	}
}

void APCharacter::CreateWeapon(int Slot, TSubclassOf<APWeapon> WeaponClass, FName SocketName)
{
	// PCore::PrintOnScreen(this, "Create Weapon", 2.f);
	
	if(!CheckWeaponIndex(Slot))
	{
		UE_LOG(LogActor, Warning, TEXT("CreateWeapon index out of range"))
		return;
	}
	APWeapon** WeaponPtr = &Weapons[Slot];

	// attach rifle
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 初始化武器
	*WeaponPtr = GetWorld()->SpawnActor<APWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	APWeapon* Weapon = *WeaponPtr;
	if(Weapon)
	{
		APlayerController* PlayerController = GetController<APlayerController>();
		Weapon->SetOwner(this);
		// Weapon->SetPlayerController(PlayerController);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		Weapon->bCanSee = false;

		// 初始化弹药数量
		Weapon->RemainBulletCount = Weapon->MaxBulletCount;
	}
	else
	{
		PCore::PrintOnScreen(this, "Create Failed", 2.f);
	}
}

UMainSceneWidget* APCharacter::GetMainSceneWidget() const
{
	return MainSceneWidget;
}

void APCharacter::SetMainSceneWidget(UMainSceneWidget* Widget)
{
	if(GetLocalRole() < ROLE_Authority)
		MainSceneWidget = Widget;
}

void APCharacter::Reload_Implementation()
{
	// PCore::PrintOnScreen(this, "Reload", 2.f);
	bool bIsInAir = GetMovementComponent()->IsFalling();
	if(bDied || !GetCurrentWeapon() || bIsFiring || bIsInAir)return;
	
	UAnimInstance* AI = GetMesh()->GetAnimInstance();
	if(AI)
	{
		if(AI->Montage_IsPlaying(ReloadMontage))return;

		bIsReloading = true;

		if(!AI->Montage_IsPlaying(ReloadMontage))
		{
			PlayAnimMontage(ReloadMontage);
		}
	}
}

void APCharacter::Server_SwitchWeapon_Implementation(int Slot)
{
	// PCore::PrintOnScreen(this, "Multicast", 3.f);
	SwitchWeapon(Slot);
}

void APCharacter::SwitchWeapon_Implementation(int Slot)
{
	// PCore::PrintOnScreen(this, "Switch Weapon", 3.f);
	if(bDied || bIsFiring || bIsReloading || bIsThrowing || GetMovementComponent()->IsFalling())return;
	// 没有武器
	if(!GetWeapon(Slot))
	{
		// PCore::PrintOnScreen(this, "No Weapon", 3.f);
		return;
	}

	// 可以切换武器
	// 该武器为当前武器，收回
	if(GetWeapon(Slot) == GetCurrentWeapon())
	{
		// PCore::PrintOnScreen(this, "Current Weapon", 3.f);
		ShootWeight = 0;
		GetCurrentWeapon()->bCanSee = false;
		PutBackCurrentWeapon();
	}
	// 该武器不是当前武器
	else
	{
		// UE_LOG(LogTemp, Display, TEXT("Not Current Weapon"))
		// 没持有武器，则装备该武器
		if(!GetCurrentWeapon())
		{
			// PCore::PrintOnScreen(this, "Not Equip This Weapon", 3.f);
			SetCurrentWeaponInSlot(Slot);
			GetCurrentWeapon()->bCanSee = true;
			ShootWeight = 1;
		}
		// 持有武器，则换下当前武器再装备该武器
		else
		{
			// PCore::PrintOnScreen(this, "Equipped Other Weapon", 3.f);
			GetCurrentWeapon()->bCanSee = false;
			SetCurrentWeaponInSlot(Slot);
			GetCurrentWeapon()->bCanSee = true;
		}
	}
}

void APCharacter::Server_PickupWeapon_Implementation()
{
	PickupWeapon();
}

void APCharacter::PickupWeapon_Implementation()
{
	if(bDied || !bCanPickup || bIsFiring || bIsReloading || bIsThrowing || GetMovementComponent()->IsFalling())return;
	if(GetLocalRole() < ROLE_Authority && IsLocallyControlled())
	{
		PCore::PrintOnScreen(GetWorld(), "Pickup", 2.f);
		if(PickupSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation(), GetActorRotation());
	}
	
	// 当前手上没有武器
	if(!GetCurrentWeapon())
	{
		int EmptyIndex = -1;
		for(int i = 0; i < Weapons.Num(); i++)
		{
			if(GetWeapon(i) == nullptr)
			{
				EmptyIndex = i;
				break;
			}
		}
		
		// 武器槽没有满，放进第一个空位
		if(EmptyIndex >= 0)
		{
			if(DesiredPickupWeapon)
			{
				SetNewWeapon(EmptyIndex, DesiredPickupWeapon);
				SetCurrentWeaponInSlot(EmptyIndex);
				ShootWeight = 1;
			}
			else
			{
				UE_LOG(LogActor, Warning, TEXT("PickupWeapon DesiredPickupWeapon is null"))
			}
		}
		
		// 武器槽满了，替换第一个位置的武器，
		if(EmptyIndex == -1)
		{
			if(DesiredPickupWeapon)
			{
				// 旧武器丢到地上，并且开启覆盖事件
				APWeapon* OldWeapon = GetWeapon(0);
				OldWeapon->bCanSee = true;
				OldWeapon->SetOwner(nullptr);
				OldWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				OldWeapon->SetActorLocation(DesiredPickupWeapon->GetActorLocation());
				OldWeapon->SetActorRotation(DesiredPickupWeapon->GetActorRotation());
				OldWeapon->MeshComp->SetGenerateOverlapEvents(true);

				SetNewWeapon(0, DesiredPickupWeapon);
				SetCurrentWeaponInSlot(0);
				ShootWeight = 1;
			}
			else
			{
				UE_LOG(LogActor, Warning, TEXT("PickupWeapon DesiredPickupWeapon is null"))
			}
		}
	}
	// 当前手上有武器，替换
	else
	{
		if(DesiredPickupWeapon)
		{
			// 旧武器丢到地上，并且开启覆盖事件
			APWeapon* OldWeapon = GetCurrentWeapon();
			OldWeapon->bCanSee = true;
			OldWeapon->SetOwner(nullptr);
			OldWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			OldWeapon->SetActorLocation(DesiredPickupWeapon->GetActorLocation());
			OldWeapon->SetActorRotation(DesiredPickupWeapon->GetActorRotation());
			OldWeapon->MeshComp->SetGenerateOverlapEvents(true);

			// 替换成新武器
			SetNewWeapon(CurrentWeaponIndex, DesiredPickupWeapon);
		}
		else
		{
			UE_LOG(LogActor, Warning, TEXT("PickupWeapon DesiredPickupWeapon is null"))
		}
	}

	// 当前武器关闭覆盖事件
	DesiredPickupWeapon->MeshComp->SetGenerateOverlapEvents(false);
}

void APCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupt)
{
	if(Montage == ThrowMontage)
	{
		bIsThrowing = false;
		if(GetCurrentWeapon())
		{
			GetCurrentWeapon()->bCanSee = true;
		}
	}
	else if(Montage == FireMontage)
	{
		bIsFiring = false;
	}
	else if(Montage == ReloadMontage)
	{
		bIsReloading = false;
		SetCurrentRemainBulletCount(GetCurrentMaxBulletCount());
	}
}

void APCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(APCharacter, ShootWeight);
	DOREPLIFETIME(APCharacter, HandIKWeight);
	DOREPLIFETIME(APCharacter, bIsFiring);
	DOREPLIFETIME(APCharacter, bIsThrowing);
	DOREPLIFETIME(APCharacter, bIsReloading);
	DOREPLIFETIME(APCharacter, Weapons);
	DOREPLIFETIME(APCharacter, CurrentWeaponIndex);
	DOREPLIFETIME(APCharacter, bDied);
	DOREPLIFETIME(APCharacter, bCanPickup);
}



