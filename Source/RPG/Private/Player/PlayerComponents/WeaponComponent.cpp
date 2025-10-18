// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerComponents/WeaponComponent.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ShapeComponent.h"
#include "Interfaces/HitInterface.h"
// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeActorsToIgnore();
}

void UWeaponComponent::BindingOverlapDelegate()
{
	BladeBoxL->OnComponentBeginOverlap.AddDynamic(this, &UWeaponComponent::OnAttackCollisionOverlap);
	BladeBoxR->OnComponentBeginOverlap.AddDynamic(this, &UWeaponComponent::OnAttackCollisionOverlap);
}

void UWeaponComponent::InitializeActorsToIgnore()
{
	ActorsToIgnore.Empty();
	ActorsToIgnore.Add(GetOwner());
}

void UWeaponComponent::InitializeWeaponComponents(USceneComponent* NewTraceStartPointL, USceneComponent* NewTraceEndPointL, USceneComponent* NewTraceStartPointR, USceneComponent* NewTraceEndPointR, UShapeComponent* NewBladeBoxL, UShapeComponent* NewBladeBoxR)
{
	TraceStartPointL = NewTraceStartPointL;
	TraceEndPointL = NewTraceEndPointL;
	TraceStartPointR = NewTraceStartPointR;
	TraceEndPointR = NewTraceEndPointR;
	BladeBoxL = NewBladeBoxL;
	BladeBoxR = NewBladeBoxR;

	BladeBoxL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BladeBoxR->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BindingOverlapDelegate();
}

//called when the overlap occur (Blade & Enemy..etc)
void UWeaponComponent::OnAttackCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);

	if (OverlappedComponent == BladeBoxL)
	{
		const FVector StartL = TraceStartPointL->GetComponentLocation();
		const FVector EndL = TraceEndPointL->GetComponentLocation();

		//TArray<AActor*> ActorsToIgnore;
		//ActorsToIgnore.Add(GetOwner());
		FHitResult HitResultL;
		//ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);

		UKismetSystemLibrary::BoxTraceSingle(
			this,
			StartL,
			EndL,
			FVector(5.f, 5.f, 5.f),
			TraceStartPointL->GetComponentRotation(),
			TraceChannel,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			HitResultL,
			true
		);

		if (HitResultL.GetActor())
		{
			ActorsToIgnore.Add(HitResultL.GetActor());
			if (HitResultL.GetActor()->ActorHasTag("Enemy"))
			{
				APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
				if (PlayerCharacter)
				{
					//send info of Target to Player 
					PlayerCharacter->HandleAttack(HitResultL.GetActor());
				}
			}

			IHitInterface* HitInterface = Cast<IHitInterface>(HitResultL.GetActor());

			if (HitInterface)
			{
				HitInterface->GetHit(HitResultL.ImpactPoint, HitResultL.GetActor());
			}
		}
	}

	else if (OverlappedComponent == BladeBoxR)
	{
		const FVector StartR = TraceStartPointR->GetComponentLocation();
		const FVector EndR = TraceEndPointR->GetComponentLocation();

		//TArray<AActor*> ActorsToIgnore;
		//ActorsToIgnore.Add(GetOwner());
		FHitResult HitResultR;
		//ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);

		UKismetSystemLibrary::BoxTraceSingle(
			this,
			StartR,
			EndR,
			FVector(5.f, 5.f, 5.f),
			TraceStartPointR->GetComponentRotation(),
			TraceChannel,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			HitResultR,
			true
		);

		if (HitResultR.GetActor())
		{
			ActorsToIgnore.Add(HitResultR.GetActor());
			if (HitResultR.GetActor()->ActorHasTag("Enemy"))
			{
				APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
				if (PlayerCharacter)
				{
					PlayerCharacter->HandleAttack(HitResultR.GetActor());
				}
			}

			IHitInterface* HitInterface = Cast<IHitInterface>(HitResultR.GetActor());

			if (HitInterface)
			{
				HitInterface->GetHit(HitResultR.ImpactPoint, HitResultR.GetActor());
			}
		}
	}
}

void UWeaponComponent::OnAttackCollisionEnable(int32 BladeSide)
{
	if (BladeSide == 0) //left
	{
		BladeBoxL->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	if (BladeSide == 1) //right
	{
		BladeBoxR->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void UWeaponComponent::OnAttackCollisionDisable(int32 BladeSide)
{
	if (BladeSide == 0) //left
	{
		BladeBoxL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (BladeSide == 1) //right
	{
		BladeBoxR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

