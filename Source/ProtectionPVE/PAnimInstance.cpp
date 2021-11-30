// Fill out your copyright notice in the Description page of Project Settings.


#include "PAnimInstance.h"

#include "PCharacter.h"
#include "PCore.h"
#include "GameFramework/PawnMovementComponent.h"

void UPAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	PCharacter = Cast<APCharacter>(TryGetPawnOwner());
}

void UPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	SetAnimParams(DeltaSeconds);
	SnapLeftHandToWeapon();
}

void UPAnimInstance::SetAnimParams(float DeltaTime)
{
	if(!PCharacter)return;

	const FVector Velocity = PCharacter->GetVelocity();
	const FRotator ActorRotator = PCharacter->GetActorRotation();
	
	Speed = Velocity.Size();
	Direction = CalculateDirection(Velocity, ActorRotator);

	if(PCharacter->bInFreeView)
	{
		AimYaw = FMath::ClampAngle(PCharacter->CurrentYaw, -90.f, 90.f) * -1;
	}
	else
	{
		AimYaw = 0.f;
	}

	if(PCharacter->IsLocallyControlled())
	{
		const FRotator DeltaRotator = PCharacter->GetControlRotation() - ActorRotator;
		const FRotator DesiredRotator = FMath::RInterpTo(FRotator(0.f, AimPitch, 0.f), DeltaRotator, DeltaTime, 150.f);
		AimPitch = FMath::ClampAngle(DesiredRotator.Pitch, -90.f, 90.f);
	}
	else
	{
		AimPitch = PCharacter->GetRemoteViewPitch();
	}

	bDied = PCharacter->bDied;
	bIsInAir = PCharacter->GetMovementComponent()->IsFalling();
	bIsCrouch = PCharacter->GetMovementComponent()->IsCrouching();

	ShootWeight = FMath::FInterpTo(ShootWeight, PCharacter->ShootWeight, DeltaTime, ShootWeightSpeed);
	HandIKWeight = FMath::FInterpTo(HandIKWeight, PCharacter->HandIKWeight, DeltaTime, HandIKWeightSpeed);
}

void UPAnimInstance::SnapLeftHandToWeapon()
{
	if(PCharacter)
	{
		APWeapon* Weapon = PCharacter->GetCurrentWeapon();
		if(Weapon)
		{
			const FTransform IKTransform = Weapon->MeshComp->GetSocketTransform(Weapon->LeftHandIKSocketName);
			FVector OutLocation;
			FRotator OutRotator;
			PCharacter->GetMesh()->TransformToBoneSpace("hand_r", IKTransform.GetLocation(), IKTransform.GetRotation().Rotator(), OutLocation, OutRotator);
			LeftHandTransform.SetLocation(OutLocation);
			LeftHandTransform.SetRotation(OutRotator.Quaternion());
		}
	}
}

void UPAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(UPAnimInstance, Direction);
	DOREPLIFETIME(UPAnimInstance, Speed);
	DOREPLIFETIME(UPAnimInstance, AimYaw);
	DOREPLIFETIME(UPAnimInstance, AimPitch);
	DOREPLIFETIME(UPAnimInstance, LeftHandTransform);
	DOREPLIFETIME(UPAnimInstance, bDied);
	DOREPLIFETIME(UPAnimInstance, bIsInAir);
	DOREPLIFETIME(UPAnimInstance, bIsCrouch);
	DOREPLIFETIME(UPAnimInstance, ShootWeight);
	DOREPLIFETIME(UPAnimInstance, HandIKWeight);
}

