#include "Component/Player/KickComponent.h"
#include "Interface/HitInterface.h"
#include "Components/ShapeComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UKickComponent::UKickComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UKickComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeActorsToIgnore();
}

void UKickComponent::OnAttackCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = TraceStartPoint->GetComponentLocation();
	const FVector End = TraceEndPoint->GetComponentLocation();

	FHitResult HitResult;
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1); //PlayerAttack Trace Channel

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		LegTraceSize,
		TraceStartPoint->GetComponentRotation(),
		TraceChannel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true,
		FLinearColor::Blue
	);

	if (HitResult.GetActor())
	{
		ActorsToIgnore.Add(HitResult.GetActor());
		IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor());

		if (HitInterface)
		{
			HitInterface->GetHit(HitResult.ImpactPoint, HitResult.GetActor());
		}
	}
}

void UKickComponent::BindingOverlapDelegate()
{
	LegBox->OnComponentBeginOverlap.AddDynamic(this, &UKickComponent::OnAttackCollisionOverlap);
}

void UKickComponent::OnAttackCollisionEnable(int32 Unused)
{
	LegBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UKickComponent::OnAttackCollisionDisable(int32 Unused)
{
	LegBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UKickComponent::InitializeActorsToIgnore()
{
	ActorsToIgnore.Empty();
	ActorsToIgnore.Add(GetOwner());
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


