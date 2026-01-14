// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/Boss/BossAttackComponent.h"

// Sets default values for this component's properties
UBossAttackComponent::UBossAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UBossAttackComponent::OnAttackCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnAttackCollisionOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
		SweepResult);
}



// Called when the game starts
void UBossAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

