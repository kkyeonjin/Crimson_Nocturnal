// Manage kick collision used by player (kick attack with Y Button)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/AttackCollisionInterface.h"
#include "KickComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UKickComponent : public UActorComponent, public IAttackCollisionInterface
{
	GENERATED_BODY()

public:
	UKickComponent();

	UFUNCTION()
	virtual void OnAttackCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void BindingOverlapDelegate() override;


	// Callback funcs by AnimNotify
	UFUNCTION()
	virtual void OnAttackCollisionEnable(int32 Unused) override;
	UFUNCTION()
	virtual void OnAttackCollisionDisable(int32 Unused) override;

	virtual void InitializeActorsToIgnore() override;

protected:
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

	UPROPERTY(EditAnywhere, Category="Kick")
	FVector LegTraceSize;
};
