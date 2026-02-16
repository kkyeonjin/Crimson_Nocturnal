// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Boss/PhaseManageComponent.h"


// Sets default values for this component's properties
UPhaseManageComponent::UPhaseManageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}


// Called when the game starts
void UPhaseManageComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AvailablePhases.Num() == 0)
	{
		AvailablePhases.Add(EBossPhase::Phase1_MagicCircle);
		AvailablePhases.Add(EBossPhase::Phase2_WingAttack);
		AvailablePhases.Add(EBossPhase::Phase3_GroundSlam);

		UE_LOG(LogTemp, Warning, TEXT("AvailablePhases initialized in BeginPlay"));
	}

	UE_LOG(LogTemp, Warning, TEXT("AvailablePhases Num: %d"), AvailablePhases.Num());
}


EBossPhase UPhaseManageComponent::SelectRandomPhase()
{
	if (AvailablePhases.Num() == 0)
	{
		return CurrentPhase;
	}

	// int32 인덱스는 로컬 변수로만 사용
	int32 RandomIndex = FMath::Rand() % AvailablePhases.Num();

	EBossPhase NextPhase = AvailablePhases[RandomIndex];

	// 중복 방지
	if (bPreventConsecutivePhases)
	{
		int32 RetryCount = 0;
		//무한 루프 방지 위해 MaxTries 10회로 제한
		const int32 MaxRetries = 10;

		while (NextPhase == CurrentPhase && RetryCount < MaxRetries)
		{
			// 여전히 int32 인덱스는 로컬에만 사용
			RandomIndex = FMath::Rand() % AvailablePhases.Num();
			NextPhase = AvailablePhases[RandomIndex];

			RetryCount++;
		}
	}

	// Enum으로 반환
	return NextPhase;
}

void UPhaseManageComponent::ExecuteCurrentPhase()
{
	if (bIsPhaseExecutionEnable)
	{
		//CurrentPhase = SelectRandomPhase();
		CurrentPhase = EBossPhase::Phase3_GroundSlam;
		//For Setting CurrentPhase && Play Animation through calling AnimInstance in Boss class
		OnPhaseChange.Broadcast(CurrentPhase);

		switch (CurrentPhase)
		{
		case EBossPhase::Phase1_MagicCircle:
			Phase1();
			//StartPhaseTimer();
			break;

		case EBossPhase::Phase2_WingAttack:
			Phase2();
			//StartPhaseTimer();
			break;

		case EBossPhase::Phase3_GroundSlam:
			//Phase3();
			//StartPhaseTimer();
			break;

		default:
			break;
		}

		StartPhaseTimer();
	}
}


void UPhaseManageComponent::Phase1()
{
	UE_LOG(LogTemp, Warning, TEXT("Phase1_MagicCircle"));

	if (!MagicCircleAoEClass)
	{
		UE_LOG(LogTemp, Error, TEXT("MagicCircleAoEClass is not assigned!"));
		return;
	}

	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() is NULL!"));
		return;
	}

	if (MagicCircleAoEClass && GetWorld())
	{
		GetWorld()->SpawnActor<AActor>(
			MagicCircleAoEClass,
			Center,
			FRotator::ZeroRotator);
	}
}

void UPhaseManageComponent::Phase2()
{
	UE_LOG(LogTemp, Warning, TEXT("Phase2_WingAttack"));

	if (WingAttackClass && GetWorld())
	{
		GetWorld()->SpawnActor<AActor>(
			WingAttackClass,
			Center,
			FRotator::ZeroRotator);
	}
}

void UPhaseManageComponent::Phase3()
{
	UE_LOG(LogTemp, Warning, TEXT("Phase3_GroundSlam"));

	if (GroundSlamClass && GetWorld())
	{
		GetWorld()->SpawnActor<AActor>(
			GroundSlamClass,
			Center,
			FRotator::ZeroRotator);
	}
}

void UPhaseManageComponent::UltimateAttack()
{
	//CurrentPhase = EBossPhase::Ultimate;

	//OnPhaseChange.Broadcast(CurrentPhase);


}

void UPhaseManageComponent::StartPhaseTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("StartPhaseTimer"));

	//PhaseTimer 시작
	//끝나면 ExecuteCurrentPhase 시작
	float Duration = 5.f;

	switch (CurrentPhase)
	{
	case EBossPhase::Phase1_MagicCircle:
		Duration = Phase1Duration;
		break;
	case EBossPhase::Phase2_WingAttack:
		Duration = Phase2Duration;
		break;
	case EBossPhase::Phase3_GroundSlam:
		Duration = Phase3Duration;
		break;
	default:
		break;
	}

	if (GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("StartPhaseTimer"));

		GetWorld()->GetTimerManager().SetTimer(
			PhaseTimer,
			this,
			&UPhaseManageComponent::ExecuteCurrentPhase,
			Duration,
			false
		);
	}
}

