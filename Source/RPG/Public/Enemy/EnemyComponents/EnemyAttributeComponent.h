// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/AttributeInterface.h"
#include "EnemyAttributeComponent.generated.h"

class UPlayerAttributeComponent;
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UEnemyAttributeComponent : public UActorComponent, public IAttributeInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyAttributeComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual float CalcDamage() override;
	virtual void ApplyDamage() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UDataTable* EnemyStatTable;

private:

	float ATK;
	float DEF;
	float CurrentHP;

	UPlayerAttributeComponent* PlayerAttributeComponent;

	void InitializeEnemyStat();

public:
	FORCEINLINE virtual float GetATK() override { return ATK; }
	FORCEINLINE virtual float GetDEF() override { return DEF; }

		
};
