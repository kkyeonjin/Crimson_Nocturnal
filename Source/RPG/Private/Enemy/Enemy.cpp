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
	if (!MeshComp) return;

	//Set OriginalMaterials
	if (OriginalMaterials.Num() == 0)
	{
		int32 NumMaterials = MeshComp->GetNumMaterials();
		for (int32 i = 0; i < NumMaterials; i++)
		{
			OriginalMaterials.Add(MeshComp->GetMaterial(i));
		}
	}

	//Create & Set DynamicMaterials
	for (int32 i = 0; i < OriginalMaterials.Num(); i++)
	{
		if (OriginalMaterials[i])
		{
			UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(OriginalMaterials[i], this);
			DynamicMaterials.Add(DynMat);

			MeshComp->SetMaterial(i, DynMat);

			UE_LOG(LogTemp, Display, TEXT("[%s] Dynamic Material created"), *GetName());
		}
	}

	CurrentAppliedMaterials = DynamicMaterials;

	//create Dynamic Red Material
	DynRedMaterial = UMaterialInstanceDynamic::Create(RedMaterial, this);

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

void AEnemy::AttackStart(UBehaviorTreeComponent* BTComp, UBTTaskNode* TaskNode)
{
	if (CachedEnemyAnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] EnemyAttackStart"), *GetName());

		// Store BT Component 
		CachedBTComponent = BTComp;
		CachedBTTaskNode = TaskNode;

		CachedEnemyAnimInstance->PlayAnimMontage(0);
		CachedEnemyAnimInstance->OnEnemyAttackMontageEnd.RemoveDynamic(this, &AEnemy::OnEnemyAttackMontageEnded);
		CachedEnemyAnimInstance->OnEnemyAttackMontageEnd.AddDynamic(this, &AEnemy::OnEnemyAttackMontageEnded);
		EnemyCombatState = EEnemyCombatState::EECS_Attacking;
	}
}

void AEnemy::OnEnemyAttackMontageEnded()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] OnEnemyAttackMontageEnded"), *GetName());

	if (CachedEnemyAnimInstance)
	{
		//OnEnemyAttackEnd.Broadcast();
	}
	CachedEnemyAttackComponent->InitializeActorsToIgnore();
	EnemyCombatState = EEnemyCombatState::EECS_NotAttacking;

	// BT Task 직접 종료
	if (CachedBTComponent && CachedBTTaskNode)
	{
		CachedBTTaskNode->FinishLatentTask(*CachedBTComponent, EBTNodeResult::Succeeded);
		UE_LOG(LogTemp, Display, TEXT("[%s] BT Task Finished from Enemy"), *GetName());

		// 캐시 초기화
		CachedBTComponent = nullptr;
		CachedBTTaskNode = nullptr;
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

void AEnemy::ReadyToInfect()
{
	bReadyToInfect = true;
	UE_LOG(LogTemp, Warning, TEXT("ReadyToInfect"));
}

void AEnemy::RestoreMaterialToDefault()
{
	UE_LOG(LogTemp, Warning, TEXT("InfectTimerOver"));
	USkeletalMeshComponent* MeshComp = this->GetMesh();
	if (!MeshComp) return;

	CurrentAppliedMaterials.Empty();

	for (int32 i = 0; i < DynamicMaterials.Num(); i++)
	{
		if (DynamicMaterials[i])
		{
			MeshComp->SetMaterial(i, DynamicMaterials[i]);
			CurrentAppliedMaterials.Add(DynamicMaterials[i]);
		}
	}

	if (DynamicMaterials.Num() != 0)
	{
		
	}
}


void AEnemy::GetHit(const FVector& ImpactPoint, AActor* unused)
{
	EnemyCombatState = EEnemyCombatState::EECS_GetAttacked;

	if (bReadyToInfect && !CachedEnemyAttributeComponent->GetIsInfected())
	{
		USkeletalMeshComponent* MeshComp = this->GetMesh();
		if (MeshComp && RedMaterial)
		{
			//Initialize CurrentMaterials Array
			CurrentAppliedMaterials.Empty();

			//Set Infect Material 
			for (int i = 0; i < DynamicMaterials.Num(); i++)
			{
				MeshComp->SetMaterial(i, DynRedMaterial);
				CurrentAppliedMaterials.Add(DynRedMaterial);
			}
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

			//Stop BT
			CachedBTComponent->StopLogic(TEXT("EnemyDead"));
			//Stop Movement
			GetCharacterMovement()->StopMovementImmediately();
			GetCharacterMovement()->DisableMovement();

			StartDeathSequence();
		}
		else
		{
			CachedEnemyAnimInstance->PlayAnimMontage(1); //HitReact AnimMontage 
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

void AEnemy::StartDeathSequence()
{
	// 페이드아웃 타이머 시작 (0.05초마다 업데이트)
	GetWorldTimerManager().SetTimer(
		DeathTimer,
		this,
		&AEnemy::FadeOutAndDestroy,
		0.05f,  // 업데이트 간격
		true    // 반복
	);

	UE_LOG(LogTemp, Display, TEXT("[%s] Fade out started"), *GetName());

}

void AEnemy::FadeOutAndDestroy()
{
	// Alpha 감소 (1.0 → 0.0)
	FadeOutAlpha -= (0.05f / FadeOutDuration);  // Delta = 업데이트 간격 / 전체 시간

	if (FadeOutAlpha <= 0.0f)
	{
		FadeOutAlpha = 0.0f;
		GetWorldTimerManager().ClearTimer(DeathTimer);

		UE_LOG(LogTemp, Display, TEXT("[%s] Fade out complete, destroying"), *GetName());
		Destroy();
		return;
	}

	for (UMaterialInstanceDynamic* DynMat : CurrentAppliedMaterials)
	{
		if (DynMat)
		{
			DynMat->SetScalarParameterValue(FName("FadeAlpha"), FadeOutAlpha);
		}
	}
}




