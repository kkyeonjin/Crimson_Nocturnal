#include "Character/Boss/Boss.h"
#include "Actor/Crystal.h"
#include "Actor/BossUltimateSkill.h"
#include "Component/Boss/BossAttributeComponent.h"
#include "Character/Boss/BossAnimInstance.h"
#include "Component/Boss/PhaseManageComponent.h"
#include "Component/Common/EnemyAttackComponent.h"
#include "Component/Common/InfectionComponent.h"
#include "Component/Boss/BossSFXComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Game/RPGGameInstance.h"
#include "Kismet/GameplayStatics.h"

ABoss::ABoss()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	InitializeMaterials();

	if (InitializeComponents())
	{
		BindDelegates();
	}
}

bool ABoss::InitializeComponents()
{
	CachedBossAnimInstance = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());

	TArray<UActorComponent*> Components = {
		CachedBossAttributeComponent = FindComponentByClass<UBossAttributeComponent>(),
		CachedPhaseManageComponent = FindComponentByClass<UPhaseManageComponent>(),
		CachedInfectionComponent = FindComponentByClass<UInfectionComponent>(),
		CachedBossSFXComponent = FindComponentByClass<UBossSFXComponent>(),
		CachedAttackComponent = FindComponentByClass<UEnemyAttackComponent>()
	};

	for (auto* Comp : Components)
		if (!Comp) return false;

	return true;
}

void ABoss::BindDelegates()
{
	check(CachedPhaseManageComponent);
	check(CachedBossAnimInstance);
	check(CachedInfectionComponent);
	check(CachedAttackComponent);
	check(CachedBossAttributeComponent);
	check(Crystals.Num() >= 3);  

	CachedPhaseManageComponent->OnPhaseChange.AddDynamic(this, &ABoss::OnPhaseChange);

	CachedBossAnimInstance->OnBattleStart.AddDynamic(this, &ABoss::StartBattle);
	CachedBossAnimInstance->OnSlamActorSpawn.AddDynamic(CachedPhaseManageComponent, &UPhaseManageComponent::Phase3);
	CachedBossAnimInstance->OnBossCollisionEnable.AddDynamic(this, &ABoss::OnCollisionEnable);
	CachedBossAnimInstance->OnBossCollisionDisable.AddDynamic(this, &ABoss::OnCollisionDisable);
	CachedBossAnimInstance->OnBossDead.AddDynamic(this, &ABoss::StartEnding);

	CachedInfectionComponent->OnInfectionStarted.AddDynamic(this, &ABoss::OnInfectionStarted);
	CachedInfectionComponent->OnInfectionEnded.AddDynamic(this, &ABoss::OnInfectionEnded);

	CachedBossAttributeComponent->OnHpThreshold.AddDynamic(this, &ABoss::OnHpThreshold);

	for (int32 i = 2; i < Crystals.Num(); ++i)
	{
		if (ACrystal* Crystal = Crystals[i])
		{
			Crystal->OnLastCrystalDestroyed.AddDynamic(this, &ABoss::OnLastCrystalDestroyed);
			UE_LOG(LogTemp, Error, TEXT("Crystal %d Binded!!!"), i);
		}
	}
}

void ABoss::InitializeMaterials()
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) return;

	// collect original materials
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

	// create DynamicMaterials 
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

//First Func that starts
void ABoss::OnBattleTriggered()
{
	//Play BattleStartMotion
	CachedBossAnimInstance->PlayBossAnimMontage(0);
	CachedBossSFXComponent->PlayBattleStartSFX();

	if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetWorld()->GetGameInstance()))
	{
		GI->PlayBossBGM();  
	}
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
	for (ACrystal* Crystal : Crystals)
	{
		if (Crystal)
		{
			Crystal->DestroyCrystal(ThresholdStep);
		}
	}
}

void ABoss::OnPlayerQTESuccess()
{
	UltActor->Destroy();

	for (ACrystal* Crystal : Crystals)
	{
		if (Crystal)
		{
			Crystal->SetReadyToBreak();
		}
	}
}

//callback func when crystal destroyed by player
void ABoss::OnLastCrystalDestroyed()
{
	UE_LOG(LogTemp, Error, TEXT("Last Crystal Destroyed!!!!"));

	CachedBossAnimInstance->PlayBossAnimMontage(6); //Play death Anim
	CachedBossSFXComponent->PlayDeathSFX();
}

void ABoss::StartEnding()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Ending")));
}

void ABoss::StartBattle()
{
	CachedPhaseManageComponent->ExecuteCurrentPhase();
}

void ABoss::OnPhaseChange(EBossPhase NewPhase)
{
	CurrentPhase = NewPhase;
	int32 PhaseNum = static_cast<int32>(NewPhase);
	CachedBossAnimInstance->PlayBossAnimMontage(PhaseNum);
	CachedBossSFXComponent->PlayPhaseSkillSFX(PhaseNum);
}

void ABoss::StartUltimateMechanic()
{
	CachedBossAnimInstance->PlayBossAnimMontage(5); 
	CachedPhaseManageComponent->StopPhaseExecution();

	CachedBossSFXComponent->PlayUltSFX();

	GetWorld()->GetTimerManager().SetTimer(
		UltSkillActorTimerHandle,
		this,
		&ABoss::SpawnUltimateActor,
		2.5f,
		false
	);

	//for clear all minions left in level
	OnLastAttackStarted.Broadcast();
}


void ABoss::SpawnUltimateActor()
{
	//for Player State Change When UltSkill(QTE Mode) Starts
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());
	}

	PlayerCharacter->OnQTESuccess.AddDynamic(this, &ABoss::OnPlayerQTESuccess);

	if (UltSkillClass && GetWorld())
	{
		UltActor = GetWorld()->SpawnActor<ABossUltimateSkill>(
			UltSkillClass,
			UltSpawnPoint,
			FRotator::ZeroRotator);

		UltActor->SetTargetPlayer(PlayerCharacter);
	}
}

void ABoss::GetHit(const FVector& ImpactPoint, AActor* Target)
{
	//Infection Part (through InfectionComp & InfectionInterface)
	if (bReadyToInfect && CachedInfectionComponent)
	{
		CachedInfectionComponent->StartInfection();
		bReadyToInfect = false;
	}

	if (CachedBossAttributeComponent)
	{
		CachedBossAttributeComponent->ApplyDamage();
	}
	if (CachedBossAnimInstance)
	{
		if (CachedBossAttributeComponent->GetCurrentHP() <= 0.f && !IsUltSkillStarted)
		{
			StartUltimateMechanic();
			IsUltSkillStarted = true;
		}
		else
		{
			CachedBossAnimInstance->PlayBossAnimMontage(4); // HitReact AnimMontage 
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
}

void ABoss::OnInfectionStarted()
{
	CachedBossAttributeComponent->SetIsInfected(true);
}

void ABoss::OnInfectionEnded()
{
	CachedBossAttributeComponent->SetIsInfected(false);
	//restore material
	CurrentAppliedMaterials = DynamicMaterials;
}


void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

