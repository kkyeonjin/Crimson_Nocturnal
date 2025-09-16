// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	USceneComponent* TraceStartPointL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USceneComponent* TraceEndPointL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USceneComponent* TraceStartPointR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USceneComponent* TraceEndPointR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UShapeComponent* BladeBoxL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UShapeComponent* BladeBoxR;

	//callback func to delegate
	UFUNCTION()
	void OnBladeROverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBladeLOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void BindingOverlapDelegate();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void InitializeWeaponComponents(
		USceneComponent* NewTraceStartPointL,
		USceneComponent* NewTraceEndPointL,
		USceneComponent* NewTraceStartPointR,
		USceneComponent* NewTraceEndPointR,
		UShapeComponent* NewBladeBoxL,
		UShapeComponent* NewBladeBoxR);
};
