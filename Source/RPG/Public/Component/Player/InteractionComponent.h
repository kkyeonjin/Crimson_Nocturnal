// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

    UPROPERTY()
    AActor* CurrentInteractable = nullptr;

    UFUNCTION()
    void OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void InteractionStart();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast")
    float TraceDistance = 500.0f;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Raycast Collision")
    UShapeComponent* PlayerCapsule;

    UFUNCTION(BlueprintCallable)
    void InitializeCapsuleCollision(UShapeComponent* NewPlayerCapsule);

private:
    APlayerCharacter* PlayerCharacter;

		
};
