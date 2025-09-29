// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
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
}

void UPlayerAnimInstance::PlayAnimMontage(int32 Num)
{
	switch (Num)
	{
		//Primary Animations
		case 0:
			Montage_Play(AttackA);
			break;
		case 1:
			Montage_Play(AttackB);
			break;

		//Combo Animations
		case 10:
			Montage_Play(ComboA);
			break;
		case 11:
			Montage_Play(ComboB);
			break;

		default:
			break;
	}

}
