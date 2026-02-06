// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Enemy.h"
#include "Character/Enemy/EnemyAnimInstance.h"
#include "Component/Common/EnemyAttackComponent.h"
#include "Component/Enemy/EnemyAttributeComponent.h"
#include "Component/Common/InfectionComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundCue.h"
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


	InitializeMaterials();

	if (InitializeComponents())
	{
		//bind delegates only when all components are found safely
		BindDelegates();
	}
	
}



//Callback Func called by InfectionComp
void AEnemy::OnInfectionStarted()
{
	CachedEnemyAttributeComponent->SetIsInfected(true);
}

void AEnemy::OnInfectionEnded()
{
	CachedEnemyAttributeComponent->SetIsInfected(false);

	CurrentAppliedMaterials = DynamicMaterials;
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

//BT, NODE 받아서 저장 (공격 시작하는 시점에) 
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

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSFX, GetActorLocation());
	}
}

void AEnemy::OnEnemyAttackMontageEnded()
{
	//UE_LOG(LogTemp, Warning, TEXT("[%s] OnEnemyAttackMontageEnded"), *GetName());

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
		//UE_LOG(LogTemp, Display, TEXT("[%s] BT Task Finished from Enemy"), *GetName());

		// 캐시 초기화
		CachedBTComponent = nullptr;
		CachedBTTaskNode = nullptr;
	}
}

void AEnemy::SetEnemyGroundSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

bool AEnemy::InitializeComponents()
{
	CachedEnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (!CachedEnemyAnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance Cast Failed in BeginPlay"));
		return false;
	}
	CachedEnemyAttributeComponent = FindComponentByClass<UEnemyAttributeComponent>();
	if (!CachedEnemyAttributeComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyAttributeComp Finding Failed in BeginPlay"));
		return false;
	}
	CachedEnemyAttackComponent = FindComponentByClass<UEnemyAttackComponent>();
	if (!CachedEnemyAttackComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyAttackComp Finding Failed in BeginPlay"));
		return false;
	}

	CachedInfectionComponent = FindComponentByClass<UInfectionComponent>();
	if (!CachedInfectionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("InfectionComp Finding Failed in BeginPlay"));
		return false;
	}

	return true;
}

void AEnemy::BindDelegates()
{
	//for debug
	check(CachedEnemyAnimInstance);
	check(CachedEnemyAttributeComponent);
	check(CachedEnemyAttackComponent);
	check(CachedInfectionComponent);

	CachedEnemyAnimInstance->OnEnemyCollisionEnable.AddDynamic(this, &AEnemy::SetAttackCollisionEnable);
	CachedEnemyAnimInstance->OnEnemyCollisionDisable.AddDynamic(this, &AEnemy::SetAttackCollisionDisable);

	CachedInfectionComponent->OnInfectionStarted.AddDynamic(this, &AEnemy::OnInfectionStarted);
	CachedInfectionComponent->OnInfectionEnded.AddDynamic(this, &AEnemy::OnInfectionEnded);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//Callback Func called by player when player match combos & Skill C is Activated
void AEnemy::ReadyToInfect()
{
	bReadyToInfect = true;
	UE_LOG(LogTemp, Warning, TEXT("ReadyToInfect"));
}

TArray<UMaterialInstanceDynamic*> AEnemy::GetOriginalDynamicMaterials_Implementation()
{
	return DynamicMaterials;
}

void AEnemy::SetCurrentMaterial_Implementation(UMaterialInstanceDynamic* Material)
{
	CurrentAppliedMaterials.Empty();
	CurrentAppliedMaterials.Add(Material);
}

USkeletalMeshComponent* AEnemy::GetMeshComponent_Implementation()
{
	return GetMesh();
}

void AEnemy::GetHit(const FVector& ImpactPoint, AActor* unused)
{
	EnemyCombatState = EEnemyCombatState::EECS_GetAttacked;

	//Infection Part (through InfectionComp & InfectionInterface)
	if (bReadyToInfect && CachedInfectionComponent)
	{
		// InfectionComponent가 Interface를 통해 자동으로 처리
	   // (이제 Enemy가 감염 머티리얼 로직을 직접 하지 않음)
		CachedInfectionComponent->StartInfection();
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

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSFX, GetActorLocation());

			OnEnemyDefeated.Broadcast(this);

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
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSFX, GetActorLocation());
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

void AEnemy::InitializeMaterials()
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) return;

	// ==================== OriginalMaterials 수집 ====================
	if (OriginalMaterials.Num() == 0)
	{
		int32 NumMaterials = MeshComp->GetNumMaterials();
		for (int32 i = 0; i < NumMaterials; i++)
		{
			UMaterialInterface* Mat = MeshComp->GetMaterial(i);
			if (Mat)
			{
				OriginalMaterials.Add(Mat);
			}
		}
		UE_LOG(LogTemp, Display, TEXT("[%s] Collected %d original materials"),
			*GetName(), NumMaterials);
	}

	// ==================== DynamicMaterials 생성 및 적용 ====================
	for (int32 i = 0; i < OriginalMaterials.Num(); i++)
	{
		if (OriginalMaterials[i])
		{
			UMaterialInstanceDynamic* DynMat =
				UMaterialInstanceDynamic::Create(OriginalMaterials[i], this);
			DynamicMaterials.Add(DynMat);
			MeshComp->SetMaterial(i, DynMat);

			UE_LOG(LogTemp, Display, TEXT("[%s] DynamicMaterial[%d] created"),
				*GetName(), i);
		}
	}

	CurrentAppliedMaterials = DynamicMaterials;

}

FOnCharacterDefeated& AEnemy::OnCharacterDefeatedDelegate()
{
	return OnEnemyDefeated;
}


