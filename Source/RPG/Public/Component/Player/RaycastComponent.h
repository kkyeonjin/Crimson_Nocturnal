// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RaycastComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOn)

class APlayerCharacter;

UCLASS(ClassGroup = (Raycast), meta = (BlueprintSpawnableComponent))
class RPG_API URaycastComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URaycastComponent();

    UFUNCTION()
    void OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

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
