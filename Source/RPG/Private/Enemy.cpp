// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	EnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (!EnemyAnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance Cast Failed in BeginPlay"));
	}

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::AttackStart()
{
	if (EnemyAnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyAttackStart"));
		EnemyAnimInstance->PlayAnimMontage();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyAttackFailed"));
	}
}


void AEnemy::SetEnemyGroundSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

}


// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

