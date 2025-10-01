// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerComponents/KickComponent.h"
#include "Interfaces/HitInterface.h"
#include "Components/ShapeComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UKickComponent::UKickComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKickComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKickComponent::OnKickOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = TraceStartPoint->GetComponentLocation();
	const FVector End = TraceEndPoint->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult HitResult;
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(2.f, 2.f, 2.f),
		TraceStartPoint->GetComponentRotation(),
		TraceChannel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	if (HitResult.GetActor())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor());

		if (HitInterface)
		{
			HitInterface->GetHit(HitResult.ImpactPoint);
		}
	}
	
}

void UKickComponent::BindingOverlapDelegate()
{
	LegBox->OnComponentBeginOverlap.AddDynamic(this, &UKickComponent::OnKickOverlap);
}

void UKickComponent::InitializeKickComponents(USceneComponent* NewTraceStartPoint, USceneComponent* NewTraceEndPoint,
	UShapeComponent* NewLegBox)
{
	TraceStartPoint = NewTraceStartPoint;
	TraceEndPoint = NewTraceEndPoint;
	LegBox = NewLegBox;

	LegBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BindingOverlapDelegate();
}


// Called every frame
void UKickComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

