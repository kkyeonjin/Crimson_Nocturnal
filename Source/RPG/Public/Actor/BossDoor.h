// Door that close when player enter boss fight zone
// manage door's location (closing anim)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossDoor.generated.h"

UCLASS()
class RPG_API ABossDoor : public AActor
{
	GENERATED_BODY()

public:
    ABossDoor();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* DoorMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Settings")
    FVector ClosedPosition = FVector(0, 0, -300);  

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Settings")
    float DoorAnimationDuration = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Settings")
    bool bDebugLog = true;

    UFUNCTION(BlueprintCallable, Category = "Door")
    void CloseDoor();

private:
    FTimerHandle DoorTimer;
    float DoorAnimationElapsedTime = 0.0f;
    bool bIsAnimating = false;
    FVector OpenPosition;

};
