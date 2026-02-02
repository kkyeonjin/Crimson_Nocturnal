// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Boss/Boss.h"
#include "Actor/Crystal.h"
//#include "Enemy/Boss/BossUltimateSkillActor.h"
#include "Component/Boss/BossAttributeComponent.h"
#include "Character/Boss/BossAnimInstance.h"
#include "Component/Boss/PhaseManageComponent.h"
#include "Component/Common/EnemyAttackComponent.h"
#include "Component/Common/InfectionComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABoss::ABoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	InitializeMaterials();

	if (InitializeComponents())
	{
		//bind delegates only when all components are found safely
		BindDelegates();
	}


	//For Debug
	GetWorld()->GetTimerManager().SetTimer(
		DebugTimer,
		this,
		&ABoss::StartUltimateMechanic,
		3.f,
		false
	);
	//StartUltimateMechanic();

}

bool ABoss::InitializeComponents()
{
	CachedBossAnimInstance = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());
	if (!CachedBossAnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance Cast Failed in BeginPlay"));
		return false;
	}
	CachedBossAttributeComponent = FindComponentByClass<UBossAttributeComponent>();
	if (!CachedBossAttributeComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("BossAttributeComp Finding Failed in BeginPlay"));
		return false;
	}
	CachedPhaseManageComponent = FindComponentByClass<UPhaseManageComponent>();
	if (!CachedPhaseManageComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("PhaseManageComp Finding Failed in BeginPlay"));
		return false;
	}
	CachedAttackComponent = FindComponentByClass<UEnemyAttackComponent>();
	if (!CachedAttackComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("BossAttackComp Finding Failed in BeginPlay"));
	}
	CachedInfectionComponent = FindComponentByClass<UInfectionComponent>();
	if (!CachedInfectionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("InfectionComp Finding Failed in BeginPlay"));
		return false;
	}

	return true;
}

void ABoss::BindDelegates()
{
	// check()는 실패하면 크래시 (DEBUG에서만)
	check(CachedPhaseManageComponent);
	check(CachedBossAnimInstance);
	check(CachedInfectionComponent);
	check(CachedAttackComponent);
	check(CachedBossAttributeComponent);
	
	check(Crystal);

	CachedPhaseManageComponent->OnPhaseChange.AddDynamic(this, &ABoss::OnPhaseChange);

	CachedBossAnimInstance->OnBattleStart.AddDynamic(this, &ABoss::StartBattle);
	CachedBossAnimInstance->OnBossCollisionEnable.AddDynamic(this, &ABoss::OnCollisionEnable);
	CachedBossAnimInstance->OnBossCollisionDisable.AddDynamic(this, &ABoss::OnCollisionDisable);
	CachedBossAnimInstance->OnBossDead.AddDynamic(this, &ABoss::StartEnding);

	CachedInfectionComponent->OnInfectionStarted.AddDynamic(this, &ABoss::OnInfectionStarted);
	CachedInfectionComponent->OnInfectionEnded.AddDynamic(this, &ABoss::OnInfectionEnded);

	CachedBossAttributeComponent->OnHpThreshold.AddDynamic(this, &ABoss::OnHpThreshold);

	Crystal->OnLastCrystalDestroyed.AddDynamic(this, &ABoss::OnLastCrystalDestroyed);
}

void ABoss::InitializeMaterials()
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

USkeletalMeshComponent* ABoss::GetMeshComponent_Implementation()
{
	return GetMesh();
}

TArray<UMaterialInstanceDynamic*> ABoss::GetOriginalDynamicMaterials_Implementation()
{
	return DynamicMaterials;
}

void ABoss::SetCurrentMaterial_Implementation(UMaterialInstanceDynamic* Material)
{
	CurrentAppliedMaterials.Empty();
	CurrentAppliedMaterials.Add(Material);
}

void ABoss::OnBattleTriggered()
{
	//Play BattleStartMotion
	CachedBossAnimInstance->PlayBossAnimMontage(0);
}

void ABoss::OnCollisionEnable()
{
	CachedAttackComponent->OnAttackCollisionEnable(-1);
}

void ABoss::OnCollisionDisable()
{
	CachedAttackComponent->OnAttackCollisionDisable(-1);
}

void ABoss::OnHpThreshold(int32 ThresholdStep)
{
	Crystal->DestroyCrystal(ThresholdStep);
}

void ABoss::OnPlayerQTESuccess()
{
	//Destroy Ult Skill Actor


	//Crystal->SetReadyToBreak(true);
}

//callback func when crystal destroyed by player
void ABoss::OnLastCrystalDestroyed()
{
	//죽음 애니메이션 재생 / 연출
	CachedBossAnimInstance->PlayBossAnimMontage(6);
}

void ABoss::StartEnding()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Ending")));
}

void ABoss::StartBattle()
{
	UE_LOG(LogTemp, Display, TEXT("BattleSTart"));
	//CachedPhaseManageComponent->Phase3();
	CachedPhaseManageComponent->ExecuteCurrentPhase();
}

void ABoss::EndBattle()
{

}

void ABoss::OnPhaseChange(EBossPhase NewPhase)
{
	CurrentPhase = NewPhase;
	switch (CurrentPhase)
	{
	case EBossPhase::Phase1_MagicCircle:
		UE_LOG(LogTemp, Display, TEXT("Phase 1 - Magic Circle AoE"));
		CachedBossAnimInstance->PlayBossAnimMontage(1);
		break;

	case EBossPhase::Phase2_WingAttack:
		UE_LOG(LogTemp, Display, TEXT("Phase 2 - Wing Attack"));
		CachedBossAnimInstance->PlayBossAnimMontage(2);
		break;

	case EBossPhase::Phase3_GroundSlam:
		UE_LOG(LogTemp, Display, TEXT("Phase 3 - Slam"));
		CachedBossAnimInstance->PlayBossAnimMontage(3);
		break;

	default:
		break;

	}
}

void ABoss::StartUltimateMechanic()
{

	//UltSkill 애니메이션 재생(포효하는듯한,,,암튼 먼가 으아아아 한거)
	//CachedBossAnimInstance->PlayBossAnimMontage(4); //임시로 넣은 값

	//사운드 재생
	/*
	GetWorld()->GetTimerManager().SetTimer(
		UltimateRoarSoundTimerHandle,
		this,
		&ABoss::PlayUltimateRoarSound,
		1.5f,
		false
	);
	*/

	//액터 스폰
	GetWorld()->GetTimerManager().SetTimer(
		UltSkillActorTimerHandle,
		this,
		&ABoss::SpawnUltimateActor,
		2.5f,
		false
	);

	//for minion clear
	OnLastAttackStarted.Broadcast();

}


void ABoss::SpawnUltimateActor()
{
	UE_LOG(LogTemp, Warning, TEXT("UltSkillLoading,,,,,"));


	//for Player State Change When UltSkill Starts
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());
		UE_LOG(LogTemp, Warning, TEXT("Player Find"));
	}

	//QTE성공 시 Broadcast 받을 콜백함수 연결 
	PlayerCharacter->OnQTESuccess.AddDynamic(this, &ABoss::OnPlayerQTESuccess);

	//Player가 QTE Event 실행할 준비 하도록 함 
	PlayerCharacter->StartBossQTEMode();

	/*
	//액터 스폰
	if (UltSkillActorClass && GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("UltSkillActor Spawn"));

		ABossUltimateSkillActor* UltActor = GetWorld()->SpawnActor<ABossUltimateSkillActor>(
			UltSkillActorClass,
			UltSpawnPoint,
			FRotator::ZeroRotator);

		UltActor->SetTargetPlayer(PlayerCharacter);
	}
	*/
}



void ABoss::GetHit(const FVector& ImpactPoint, AActor* Target)
{
	//Infection Part (through InfectionComp & InfectionInterface)
	if (bReadyToInfect && CachedInfectionComponent)
	{
		// InfectionComponent가 Interface를 통해 자동으로 처리
	   // (이제 Enemy가 감염 머티리얼 로직을 직접 하지 않음)
		CachedInfectionComponent->StartInfection();
		bReadyToInfect = false;
	}


	UE_LOG(LogTemp, Display, TEXT("EnemyHit!!!"));
	if (CachedBossAttributeComponent)
	{
		CachedBossAttributeComponent->ApplyDamage();
	}
	if (CachedBossAnimInstance)
	{
		if (CachedBossAttributeComponent->ReadyToStartUlt()) //필살기 발동 조건 걸리면
		{

		}
		if (CachedBossAttributeComponent->GetCurrentHP() <= 0.f)
		{
			//UltSkill Start(Spawn Particle) When HP gets 0
			StartUltimateMechanic();
			//CachedPhaseManageComponent->UltimateAttack();

			//StartDeathSequence();
		}
		else
		{
			CachedBossAnimInstance->PlayBossAnimMontage(1); //HitReact AnimMontage 
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

void ABoss::ReadyToInfect()
{
	bReadyToInfect = true;
	UE_LOG(LogTemp, Warning, TEXT("ReadyToInfect"));
}

void ABoss::OnInfectionStarted()
{
	CachedBossAttributeComponent->SetIsInfected(true);
}

void ABoss::OnInfectionEnded()
{
	CachedBossAttributeComponent->SetIsInfected(false);

	CurrentAppliedMaterials = DynamicMaterials;
}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

