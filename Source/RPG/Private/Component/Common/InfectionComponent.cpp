// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Common/InfectionComponent.h"

#include "Interface/InfectableInterface.h"

UInfectionComponent::UInfectionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UInfectionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ==================== Owner 캐시 ====================
	InfectableOwner = GetOwner();


	//create Dynamic Red Material
	DynInfectMaterial = UMaterialInstanceDynamic::Create(InfectMaterial, this);
}

void UInfectionComponent::StartInfection()
{
	//bIsInfected = true;

	ApplyInfectMaterial();

	//Infect Broadcast to Enemy (For Damage x2, Set CurrentMaterials)
	OnInfectionStarted.Broadcast();

	//Start Infection Timer
	GetWorld()->GetTimerManager().SetTimer(
		InfectTimerHandle,
		this,
		&UInfectionComponent::EndInfection,
		InfectDuration,
		false
	);

}

//감염 머티리얼 적용
void UInfectionComponent::ApplyInfectMaterial()
{
	USkeletalMeshComponent* Mesh = GetMeshComponent();  // ? 헬퍼 함수 사용
	if (!Mesh) return;

	
	// ==================== 감염 머티리얼을 모든 슬롯에 적용 ====================
	for (int32 i = 0; i < Mesh->GetNumMaterials(); i++)
	{
		Mesh->SetMaterial(i, DynInfectMaterial);
	}

	UE_LOG(LogTemp, Display,
		TEXT("[%s] Infection material applied to %d slots"),
		*GetOwner()->GetName(), Mesh->GetNumMaterials());

	//Owner의 CurrentAppliedMaterial 업데이트
	IInfectableInterface::Execute_SetCurrentMaterial(InfectableOwner, DynInfectMaterial);
	
}

void UInfectionComponent::EndInfection()
{
	//bIsInfected = false;
	//InfectionElapsedTime = 0.0f;

	UE_LOG(LogTemp, Warning,
		TEXT("[%s] Infection ended"),
		*GetOwner()->GetName());

	// ==================== 1단계: 원본 머티리얼 복구 ====================
	RestoreOriginalMaterial();

	// ==================== 2단계: AttributeComponent에 감염 해제 알림 
	OnInfectionEnded.Broadcast();
}

void UInfectionComponent::RestoreOriginalMaterial()
{
	
	USkeletalMeshComponent* Mesh = GetMeshComponent();  // ? 헬퍼 함수 사용
	if (!Mesh) return;
	

	// ==================== Interface를 통해 원본 머티리얼 획득 ====================
	
	TArray<UMaterialInstanceDynamic*> OriginalMaterials =
		IInfectableInterface::Execute_GetOriginalDynamicMaterials(InfectableOwner);

	if (OriginalMaterials.Num() == 0)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[%s] Cannot restore material: OriginalMaterials is empty"),
			*GetOwner()->GetName());
		return;
	}

	// ==================== 원본 머티리얼 복구 ====================
	for (int32 i = 0; i < OriginalMaterials.Num(); i++)
	{
		if (OriginalMaterials[i])
		{
			//Materials are all Dynamics
			Mesh->SetMaterial(i, OriginalMaterials[i]);
		}
	}

	UE_LOG(LogTemp, Display,
		TEXT("[%s] Original materials restored (%d slots)"),
		*GetOwner()->GetName(), OriginalMaterials.Num());
		

}

bool UInfectionComponent::ValidateInfectable()
{
    if (!InfectableOwner)
    {
        UE_LOG(LogTemp, Error, TEXT("[InfectionComponent] InfectableOwner is null"));
        return false;
    }

	
    if (!InfectableOwner->Implements<UInfectableInterface>())
    {
        UE_LOG(LogTemp, Error, TEXT("[%s] does not implement IInfectableInterface"),
            *InfectableOwner->GetName());
        return false;
    }
	

    return true;
}

USkeletalMeshComponent* UInfectionComponent::GetMeshComponent()
{
    if (!ValidateInfectable())
    {
        return nullptr;
    }

	
    USkeletalMeshComponent* Mesh = IInfectableInterface::Execute_GetMeshComponent(InfectableOwner);

    if (!Mesh)
    {
        UE_LOG(LogTemp, Error, TEXT("[%s] Mesh component is null"), *InfectableOwner->GetName());
    }

    return Mesh;
}


