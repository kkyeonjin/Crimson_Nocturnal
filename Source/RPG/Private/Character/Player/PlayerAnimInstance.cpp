// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player//PlayerAnimInstance.h"
#include "Character/Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	//call parent version of function
	Super::NativeInitializeAnimation();

	//find pawn Instance that currently applied on animation blueprint
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter) //casting can be failed
	{
		PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (PlayerCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerCharacterMovement->Velocity);
		IsFalling = PlayerCharacterMovement->IsFalling();
		//PlayerState = PlayerCharacter->GetPlayerState();
	}

}

void UPlayerAnimInstance::AnimNotify_AttackEnd()
{
	// 캐스팅으로 캐릭터 접근
	OnAttackMontageEnd.Broadcast();
	//UE_LOG(LogTemp, Warning, TEXT("NotifyRings"));
}

void UPlayerAnimInstance::AnimNotify_MoveEnable()
{
	OnMoveEnable.Broadcast();
	//UE_LOG(LogTemp, Warning, TEXT("NotifyRings"));
}

void UPlayerAnimInstance::AnimNotify_BladeLCollisionEnable()
{
	OnBladeLCollisionEnable.Broadcast();
}

void UPlayerAnimInstance::AnimNotify_BladeLCollisionDisable()
{
	OnBladeLCollisionDisable.Broadcast();
}

void UPlayerAnimInstance::AnimNotify_BladeRCollisionEnable()
{
	OnBladeRCollisionEnable.Broadcast();
}

void UPlayerAnimInstance::AnimNotify_BladeRCollisionDisable()
{
	OnBladeRCollisionDisable.Broadcast();
}

void UPlayerAnimInstance::AnimNotify_KickCollisionEnable()
{
	OnKickCollisionEnable.Broadcast();
	//UE_LOG(LogTemp, Warning, TEXT("NotifyRings"));
}

void UPlayerAnimInstance::AnimNotify_KickCollisionDisable()
{
	OnKickCollisionDisable.Broadcast();
	//UE_LOG(LogTemp, Warning, TEXT("NotifyRings"));
}

void UPlayerAnimInstance::AnimNotify_GameOver()
{
	OnGameOver.Broadcast();
}

void UPlayerAnimInstance::PlayAnimMontage(int32 AnimNum)
{
	//EAnimMontageType MontageType = static_cast<EAnimMontageType>(AnimNum);

	switch (AnimNum)
	{
		//Primary Animations
		
		case 0:
			Montage_Play(AttackA);
			break;
		case 1:
			Montage_Play(AttackB);
			break;
			
		case 2:
			Montage_Play(StunA);
			break;

		case 3:
			Montage_Play(Death);
			break;


		//Combo Animations
		/*
		case EAnimMontageType::ComboA:
			Montage_Play(ComboA);
			break;
		case EAnimMontageType::ComboB:
			Montage_Play(ComboB);
			break;
		case EAnimMontageType::ComboC:
			Montage_Play(ComboC);
			break;
		case EAnimMontageType::ComboD:
			Montage_Play(ComboD);
			break;
			*/

		default:
			UE_LOG(LogTemp, Warning, TEXT("Unknown animation montage: %d"), AnimNum);
			break;
	}

}

void UPlayerAnimInstance::PlayChainAttackMontage(int32 AnimNum, FName Section)
{
	switch (AnimNum)
	{
		//Primary Animations
	case 0:
		Montage_Play(AttackA);
		Montage_JumpToSection(Section, AttackA);
		break;
	case 1:
		Montage_Play(AttackB);
		Montage_JumpToSection(Section, AttackB);
		break;

	default:
		break;
	}
}

void UPlayerAnimInstance::PlayComboAttackMontage(EAnimMontageType MontageType)
{
	switch (MontageType)
	{
		case EAnimMontageType::ComboA:
			Montage_Play(ComboA);
			break;
		case EAnimMontageType::ComboB:
			Montage_Play(ComboB);
			break;
		case EAnimMontageType::ComboC:
			Montage_Play(ComboC);
			break;
		case EAnimMontageType::ComboD:
			Montage_Play(ComboD);
			break;
		default:
			break;
	}
}

