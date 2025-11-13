// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Enemy/Animation/EnemyAnimInstance.h"
#include "Enemy/EnemyComponents/EnemyAttackComponent.h"
#include "Enemy/EnemyComponents/EnemyAttributeComponent.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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

	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Player)
	{
		Player->OnSkillCReady.AddDynamic(this, &AEnemy::ReadyToInfect);
	}

	USkeletalMeshComponent* MeshComp = this->GetMesh();
	OriginalMaterial = MeshComp->GetMaterial(0);

	CachedEnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (!CachedEnemyAnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance Cast Failed in BeginPlay"));
	}
	CachedEnemyAttributeComponent = FindComponentByClass<UEnemyAttributeComponent>();
	if (!CachedEnemyAttributeComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyAttributeComp Finding Failed in BeginPlay"));
	}
	CachedEnemyAttackComponent = FindComponentByClass<UEnemyAttackComponent>();
	if (!CachedEnemyAttackComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyAttackComp Finding Failed in BeginPlay"));
	}

	CachedEnemyAnimInstance->OnEnemyCollisionEnable.AddDynamic(this, &AEnemy::SetAttackCollisionEnable);
	CachedEnemyAnimInstance->OnEnemyCollisionDisable.AddDynamic(this, &AEnemy::SetAttackCollisionDisable);
}

void AEnemy::SetAttackCollisionEnable()
{
	if (CachedEnemyAttackComponent)
	{
		CachedEnemyAttackComponent->OnAttackCollisionEnable(-1);
	}
}

void AEnemy::SetAttackCollisionDisable()
{
	if (CachedEnemyAttackComponent)
	{
		CachedEnemyAttackComponent->OnAttackCollisionDisable(-1);
	}
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
		CachedEnemyAnimInstance->OnEnemyAttackMontageEnd.RemoveDynamic(this, &AEnemy::OnEnemyAttackMontageEnded);
		CachedEnemyAnimInstance->OnEnemyAttackMontageEnd.AddDynamic(this, &AEnemy::OnEnemyAttackMontageEnded);
		//IsAttacking = true;
	}
}

void AEnemy::OnEnemyAttackMontageEnded()
{
	if (CachedEnemyAnimInstance)
	{
		OnEnemyAttackEnd.Broadcast();
	}
	EnemyCombatState = EEnemyCombatState::EECS_NotAttacking;
	CachedEnemyAttackComponent->InitializeActorsToIgnore();
	//IsAttacking = false;
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

void AEnemy::ReadyToInfect()
{
	bReadyToInfect = true;
	UE_LOG(LogTemp, Warning, TEXT("ReadyToInfect"));
}

void AEnemy::RestoreMaterialToDefault()
{
	UE_LOG(LogTemp, Warning, TEXT("InfectTimerOver"));
	USkeletalMeshComponent* MeshComp = this->GetMesh();
	MeshComp->SetMaterial(0, OriginalMaterial);
}


void AEnemy::GetHit(const FVector& ImpactPoint, AActor* unused)
{
	EnemyCombatState = EEnemyCombatState::EECS_GetAttacked;

	if (bReadyToInfect && !CachedEnemyAttributeComponent->GetIsInfected())
	{
		USkeletalMeshComponent* MeshComp = this->GetMesh();
		if (MeshComp && RedMaterial)
		{
			// 빨간 머티리얼로 교체
			MeshComp->SetMaterial(0, RedMaterial);
		}

		CachedEnemyAttributeComponent->StartInfectTimer();

		bReadyToInfect = false;
	}

	UE_LOG(LogTemp, Display, TEXT("EnemyHit!!!"));
	if (CachedEnemyAttributeComponent)
	{
		CachedEnemyAttributeComponent->ApplyDamage();
	}
	if (CachedEnemyAnimInstance)
	{
		if (CachedEnemyAttributeComponent->GetCurrentHP() <= 0.f)
		{
			CachedEnemyAnimInstance->PlayAnimMontage(2); //Play Death AnimMontage
			CachedEnemyAttributeComponent->SendEXPValue(); //Send EXP to Player that should gain
			EnemyCombatState = EEnemyCombatState::EECS_Dead;
			//bIsDead = true;
		}
		else
		{
			CachedEnemyAnimInstance->PlayAnimMontage(1);
		}
	}
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}


