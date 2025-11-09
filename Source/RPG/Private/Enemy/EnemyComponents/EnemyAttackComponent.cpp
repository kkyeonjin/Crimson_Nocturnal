// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyComponents/EnemyAttackComponent.h"
#include "Components/ShapeComponent.h"
#include "Interfaces/HitInterface.h"
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

	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UEnemyAttackComponent::OnAttackCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
		FVector(30.f, 12.f, 15.f),
		TraceStartPoint->GetComponentRotation(),
		TraceChannel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Yellow
	);

	if (HitResult.GetActor())
	{
		ActorsToIgnore.Add(HitResult.GetActor());
		IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor());

		if (HitInterface)
		{
			HitInterface->GetHit(HitResult.ImpactPoint, GetOwner());
		}
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

// Called every frame
void UEnemyAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

