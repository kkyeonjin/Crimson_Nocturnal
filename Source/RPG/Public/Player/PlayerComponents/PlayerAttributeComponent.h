// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/AttributeInterface.h"
#include "Player/Types/PlayerStat.h"
#include "PlayerAttributeComponent.generated.h"

class UEnemyAttributeComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UPlayerAttributeComponent : public UActorComponent, public IAttributeInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerAttributeComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual float CalcDamage() override;
	virtual void ApplyDamage() override;

	void SetEnemyAttributeComponent(AActor* Target);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	UDataTable* PlayerStatTable;

private:
	//to get enemy's DEF value for Damage Calculation
	UEnemyAttributeComponent* EnemyAttributeComponent;

	float ATK;
	float DEF;
	float CurrentHP;

	void InitializePlayerStat();

public:
	FORCEINLINE virtual float GetATK() override { return ATK; }
	FORCEINLINE virtual float GetDEF() override { return DEF; }
		
};
