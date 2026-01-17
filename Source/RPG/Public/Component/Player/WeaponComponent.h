// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Interface/AttackCollisionInterface.h"
#include "WeaponComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UWeaponComponent : public UActorComponent, public IAttackCollisionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponComponent();

	UFUNCTION()
	virtual void OnAttackCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	virtual void OnAttackCollisionEnable(int32 BladeSide) override;

	UFUNCTION()
	virtual void OnAttackCollisionDisable(int32 BladeSide) override;

	virtual void BindingOverlapDelegate() override;

	virtual void InitializeActorsToIgnore() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponL")
	USceneComponent* TraceStartPointL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponL")
	USceneComponent* TraceEndPointL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponR")
	USceneComponent* TraceStartPointR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponR")
	USceneComponent* TraceEndPointR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponL")
	UShapeComponent* BladeBoxL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponR")
	UShapeComponent* BladeBoxR;

	UFUNCTION(BlueprintCallable)
	void InitializeWeaponComponents(
		USceneComponent* NewTraceStartPointL,
		USceneComponent* NewTraceEndPointL,
		USceneComponent* NewTraceStartPointR,
		USceneComponent* NewTraceEndPointR,
		UShapeComponent* NewBladeBoxL,
		UShapeComponent* NewBladeBoxR);


private:
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, Category = "WeaponL")
	FVector BladeTraceSizeL;

	UPROPERTY(EditAnywhere, Category = "WeaponR")
	FVector BladeTraceSizeR;

};
