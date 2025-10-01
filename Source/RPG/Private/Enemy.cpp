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
	CachedEnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (!CachedEnemyAnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance Cast Failed in BeginPlay"));
	}
	IsAttacking = false;
}
	

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::AttackStart()
{
	if (CachedEnemyAnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyAttackStart"));

		CachedEnemyAnimInstance->PlayAnimMontage(0);
		CachedEnemyAnimInstance->OnEnemyAttackMontageEnd.AddDynamic(this, &AEnemy::OnEnemyAttackMontageEnded);
		IsAttacking = true;
	}
}

void AEnemy::OnEnemyAttackMontageEnded()
{
	if (CachedEnemyAnimInstance)
	{
		OnEnemyAttackEnd.Broadcast();
	}
	IsAttacking = false;
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

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	UE_LOG(LogTemp, Display, TEXT("EnemyHit!!!"));
	if (CachedEnemyAnimInstance)
	{
		CachedEnemyAnimInstance->PlayAnimMontage(1);
	}
	
}

