#include "Component/Boss/PhaseManageComponent.h"


UPhaseManageComponent::UPhaseManageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UPhaseManageComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AvailablePhases.Num() == 0)
	{
		AvailablePhases.Add(EBossPhase::Phase1_MagicCircle);
		AvailablePhases.Add(EBossPhase::Phase2_WingAttack);
		AvailablePhases.Add(EBossPhase::Phase3_GroundSlam);
	}

	UE_LOG(LogTemp, Warning, TEXT("AvailablePhases Num: %d"), AvailablePhases.Num());
}


EBossPhase UPhaseManageComponent::SelectRandomPhase()
{
	if (AvailablePhases.Num() == 0)
	{
		return CurrentPhase;
	}

	int32 RandomIndex = FMath::Rand() % AvailablePhases.Num();

	EBossPhase NextPhase = AvailablePhases[RandomIndex];

	// Prevent repeating same phase
	if (bPreventConsecutivePhases)
	{
		int32 RetryCount = 0;
		// Prevent infinite loop
		const int32 MaxRetries = 10;

		while (NextPhase == CurrentPhase && RetryCount < MaxRetries)
		{
			RandomIndex = FMath::Rand() % AvailablePhases.Num();
			NextPhase = AvailablePhases[RandomIndex];

			RetryCount++;
		}
	}

	return NextPhase;
}

void UPhaseManageComponent::ExecuteCurrentPhase()
{
	if (bIsPhaseExecutionEnable)
	{
		CurrentPhase = SelectRandomPhase();

		//For Setting CurrentPhase && Play Animation through calling AnimInstance in Boss class
		OnPhaseChange.Broadcast(CurrentPhase);

		switch (CurrentPhase)
		{
		case EBossPhase::Phase1_MagicCircle:
			Phase1();
			break;

		case EBossPhase::Phase2_WingAttack:
			Phase2();
			break;

		case EBossPhase::Phase3_GroundSlam:
			Phase3();
			break;

		default:
			break;
		}

		StartPhaseTimer();
	}
}


void UPhaseManageComponent::Phase1()
{
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
	if (GroundSlamClass && GetWorld())
	{
		GetWorld()->SpawnActor<AActor>(
			GroundSlamClass,
			Center,
			FRotator::ZeroRotator);
	}
}


void UPhaseManageComponent::StartPhaseTimer()
{
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
		// Start next phase when the timer is over
		GetWorld()->GetTimerManager().SetTimer(
			PhaseTimer,
			this,
			&UPhaseManageComponent::ExecuteCurrentPhase,
			Duration,
			false
		);
	}
}

