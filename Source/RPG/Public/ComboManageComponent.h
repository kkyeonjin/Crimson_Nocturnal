// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComboInput.h"
#include "ComboManageComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboComplete, int, ComboNum);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UComboManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UComboManageComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddInputKey(EAttackKeyType Key);

	UPROPERTY(BlueprintAssignable)
	FOnComboComplete OnComboComplete;

	UAnimMontage* GetComboAnimMontage(int ComboNum);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	TArray<FComboPattern> ComboPatterns;

private:
	TArray<EAttackKeyType> InputKeySequence;

	//TArray<FComboPattern> ComboPatterns;

	UPROPERTY(EditAnywhere, Category = "Combo")
	float ComboTimeOut = 2.f;

	float TimeSinceLastInput = 0.f;

	void ResetInputSequence();

	void InitializeComboPattern();

	void CheckComboTimeOut(float DeltaTime);



};
