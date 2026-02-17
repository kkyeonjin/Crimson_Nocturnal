// Manage material change & restore when enemy is infected

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InfectionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInfectionStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInfectionEnded);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UInfectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInfectionComponent();

	UPROPERTY()
	FOnInfectionStarted OnInfectionStarted;

	UPROPERTY()
	FOnInfectionEnded OnInfectionEnded;

	UFUNCTION(BlueprintCallable, Category = "Infection")
	bool IsInfected() const { return bIsInfected; }

	UFUNCTION(BlueprintCallable, Category = "Infection")
	void StartInfection();

	UFUNCTION(BlueprintCallable, Category = "Infection")
	void EndInfection();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Infection|Settings")
	float InfectDuration = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Infection|Settings")
	float InfectDamageMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Infection|VFX")
	UMaterialInterface* InfectMaterial; //Red Material

	UPROPERTY()
	UMaterialInstanceDynamic* DynInfectMaterial;

	bool bIsInfected = false;

	UPROPERTY()
	AActor* InfectableOwner;

	FTimerHandle InfectTimerHandle;

	float InfectionElapsedTime = 0.0f;

	void ApplyInfectMaterial();

	void RestoreOriginalMaterial();

	bool ValidateInfectable();
	USkeletalMeshComponent* GetMeshComponent();

};
