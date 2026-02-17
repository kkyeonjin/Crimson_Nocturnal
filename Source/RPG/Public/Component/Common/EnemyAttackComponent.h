// Manage attack collision of enemy (overlap logics)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/AttackCollisionInterface.h"
#include "EnemyAttackComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UEnemyAttackComponent : public UActorComponent, public IAttackCollisionInterface
{
	GENERATED_BODY()

public:
	UEnemyAttackComponent();

	UFUNCTION()
	virtual void OnAttackCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// Callback funcs by AnimNotify
	UFUNCTION()
	virtual void OnAttackCollisionEnable(int32 Unused) override;
	UFUNCTION()
	virtual void OnAttackCollisionDisable(int32 Unused) override;

	virtual void BindingOverlapDelegate() override;

	virtual void InitializeActorsToIgnore() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	USceneComponent* TraceStartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	USceneComponent* TraceEndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UShapeComponent* AttackCollision;

	UFUNCTION(BlueprintCallable)
	void InitializeEnemyAttackComponents(USceneComponent* NewTraceStartPoint, USceneComponent* NewTraceEndPoint, UShapeComponent* NewAttackCollision);

private:
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere)
	bool IsCollisionEnableAtStart;
	UPROPERTY(EditAnywhere)
	bool IsTraceEnable;

	UPROPERTY(EditAnywhere, Category="Attack")
	FVector AttackTraceSize;

};
