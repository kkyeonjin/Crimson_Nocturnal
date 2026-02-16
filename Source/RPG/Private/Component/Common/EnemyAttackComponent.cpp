// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Common/EnemyAttackComponent.h"
#include "Components/ShapeComponent.h"
#include "Interface/HitInterface.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UEnemyAttackComponent::UEnemyAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
		//UE_LOG(LogTemp, Display, TEXT("Overlap Start"));
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
			EDrawDebugTrace::ForDuration,
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

	// 히트 리스트에 추가 (중복 방지)
	ActorsToIgnore.AddUnique(OtherActor);

	// 데미지 적용
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

// Called when the game starts
void UEnemyAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeActorsToIgnore();

}

