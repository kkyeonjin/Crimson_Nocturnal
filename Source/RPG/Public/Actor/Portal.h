// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Portal.generated.h"

UCLASS()
class RPG_API APortal : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    APortal();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* CollisionBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* PortalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
    FName DestinationMap;

    // IInteractable 구현
    virtual void OnInteract() override;

private:
    UPROPERTY(EditAnywhere) 
    bool EnteringSaloon = false;  // Saloon으로 가는 포탈

    UPROPERTY(EditAnywhere)
    bool ExitingSaloon = false;  // Saloon에서 나가는 포탈
   
};