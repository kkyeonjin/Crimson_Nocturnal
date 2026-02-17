#include "Character/Enemy/Enemy.h"
#include "AI/EnemyAIController.h"
#include "Character/Enemy/EnemyAnimInstance.h"
#include "Component/Common/EnemyAttackComponent.h"
#include "Component/Enemy/EnemyAttributeComponent.h"
#include "Component/Common/InfectionComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
}

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

void AEnemy::AttackStart(UBehaviorTreeComponent* BTComp, UBTTaskNode* TaskNode)
{
	if (CachedEnemyAnimInstance)
	{
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
	CachedEnemyAttackComponent->InitializeActorsToIgnore();
	EnemyCombatState = EEnemyCombatState::EECS_NotAttacking;

	// Finish BT Task
	if (CachedBTComponent && CachedBTTaskNode)
	{
		CachedBTTaskNode->FinishLatentTask(*CachedBTComponent, EBTNodeResult::Succeeded);
	}
}

bool AEnemy::InitializeComponents()
{
	CachedEnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	TArray<UActorComponent*> Components = {
		CachedEnemyAttributeComponent = FindComponentByClass<UEnemyAttributeComponent>(),
		CachedEnemyAttackComponent = FindComponentByClass<UEnemyAttackComponent>(),
		CachedInfectionComponent = FindComponentByClass<UInfectionComponent>()
	};

	for (auto* Comp : Components)
		if (!Comp) return false;

	return true;
}

void AEnemy::BindDelegates()
{
	check(CachedEnemyAnimInstance);
	check(CachedEnemyAttributeComponent);
	check(CachedEnemyAttackComponent);
	check(CachedInfectionComponent);

	CachedEnemyAnimInstance->OnEnemyCollisionEnable.AddDynamic(this, &AEnemy::SetAttackCollisionEnable);
	CachedEnemyAnimInstance->OnEnemyCollisionDisable.AddDynamic(this, &AEnemy::SetAttackCollisionDisable);

	CachedInfectionComponent->OnInfectionStarted.AddDynamic(this, &AEnemy::OnInfectionStarted);
	CachedInfectionComponent->OnInfectionEnded.AddDynamic(this, &AEnemy::OnInfectionEnded);
}


void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

//Callback Func called by player when player match combos & Skill C is Activated
void AEnemy::ReadyToInfect()
{
	bReadyToInfect = true;
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
		CachedInfectionComponent->StartInfection();
		bReadyToInfect = false;
	}

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

			// 1. BT/Controller stop
			if (CachedBTComponent) CachedBTComponent->StopLogic(TEXT("EnemyDead"));

			// 2. Movement stop
			if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
			{
				MoveComp->StopMovementImmediately();
				MoveComp->DisableMovement();  
			}

			// 3. Material Fade Start
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
	GetWorldTimerManager().SetTimer(
		DeathTimer,
		this,
		&AEnemy::FadeOutAndDestroy,
		0.05f,  
		true    
	);
}

void AEnemy::FadeOutAndDestroy()
{
	FadeOutAlpha -= (0.05f / FadeOutDuration);  

	if (FadeOutAlpha <= 0.0f)
	{
		FadeOutAlpha = 0.0f;
		GetWorldTimerManager().ClearTimer(DeathTimer);
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

	// Collect Original Materials
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

	// Create Dynamic Materials 
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


