#include "Component/Common/EnemyAttackComponent.h"
#include "Components/ShapeComponent.h"
#include "Interface/HitInterface.h"
#include "Kismet/KismetSystemLibrary.h"

UEnemyAttackComponent::UEnemyAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnemyAttackComponent::InitializeEnemyAttackComponents(USceneComponent* NewTraceStartPoint, USceneComponent* NewTraceEndPoint, UShapeComponent* NewAttackCollision)
{
	TraceStartPoint = NewTraceStartPoint;
	TraceEndPoint = NewTraceEndPoint;
	AttackCollision = NewAttackCollision;

	BindingOverlapDelegate();

	if (IsCollisionEnableAtStart)
	{
		AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	}
	else
	{
		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UEnemyAttackComponent::OnAttackCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorsToIgnore.Contains(OtherActor)) return;
	if (!OtherActor || !OtherActor->ActorHasTag("Player")) return;

	FVector HitLocation;
	bool bHitValid = false;

	if (IsTraceEnable)
	{
		const FVector Start = TraceStartPoint->GetComponentLocation();
		const FVector End = TraceEndPoint->GetComponentLocation();

		FHitResult HitResult;
		ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2); //EnemyAttack Trace Channel

		UKismetSystemLibrary::BoxTraceSingle(
			this,
			Start,
			End,
			AttackTraceSize,
			TraceStartPoint->GetComponentRotation(),
			TraceChannel,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResult,
			true,
			FLinearColor::Yellow
		);

		if (HitResult.GetActor()) {
			HitLocation = HitResult.ImpactPoint;
			bHitValid = true;
		}
	}
	else
	{
		HitLocation = SweepResult.ImpactPoint;
		bHitValid = true;
	}

	if (!bHitValid) return;

	// Prevent to give damage several times
	ActorsToIgnore.AddUnique(OtherActor);

	if (IHitInterface* HitInterface = Cast<IHitInterface>(OtherActor)) 
	{
		HitInterface->GetHit(HitLocation, GetOwner());
	}
}

void UEnemyAttackComponent::OnAttackCollisionEnable(int32 Unused)
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UEnemyAttackComponent::OnAttackCollisionDisable(int32 Unused)
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UEnemyAttackComponent::BindingOverlapDelegate()
{
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &UEnemyAttackComponent::OnAttackCollisionOverlap);
}

void UEnemyAttackComponent::InitializeActorsToIgnore()
{
	ActorsToIgnore.Empty();
	ActorsToIgnore.Add(GetOwner());
}


void UEnemyAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeActorsToIgnore();

}

