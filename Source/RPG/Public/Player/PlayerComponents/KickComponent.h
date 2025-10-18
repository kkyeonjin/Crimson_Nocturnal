// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/AttackCollisionInterface.h"
#include "KickComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UKickComponent : public UActorComponent, public IAttackCollisionInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKickComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void OnAttackCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void BindingOverlapDelegate() override;

	UFUNCTION()
	virtual void OnAttackCollisionEnable(int32 Unused) override;

	UFUNCTION()
	virtual void OnAttackCollisionDisable(int32 Unused) override;

	virtual void InitializeActorsToIgnore() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kick")
	USceneComponent* TraceStartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kick")
	USceneComponent* TraceEndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kick")
	UShapeComponent* LegBox;

	UFUNCTION(BlueprintCallable)
	void InitializeKickComponents(
		USceneComponent* NewTraceStartPoint,
		USceneComponent* NewTraceEndPoint,
		UShapeComponent* NewLegBox
		);


private:
	TArray<AActor*> ActorsToIgnore;

		
};
