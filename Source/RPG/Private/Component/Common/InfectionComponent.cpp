#include "Component/Common/InfectionComponent.h"
#include "Interface/InfectableInterface.h"

UInfectionComponent::UInfectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UInfectionComponent::BeginPlay()
{
	Super::BeginPlay();

	InfectableOwner = GetOwner();

	// Create Dynamic Red Material
	DynInfectMaterial = UMaterialInstanceDynamic::Create(InfectMaterial, this);
}

void UInfectionComponent::StartInfection()
{
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

void UInfectionComponent::ApplyInfectMaterial()
{
	USkeletalMeshComponent* Mesh = GetMeshComponent();  
	if (!Mesh) return;

	// Apply infection material in every slot
	for (int32 i = 0; i < Mesh->GetNumMaterials(); i++)
	{
		Mesh->SetMaterial(i, DynInfectMaterial);
	}

	// Update Owner's CurrentAppliedMaterial 
	IInfectableInterface::Execute_SetCurrentMaterial(InfectableOwner, DynInfectMaterial);
	
}

void UInfectionComponent::EndInfection()
{
	RestoreOriginalMaterial();

	// Delegate to EnemyAttributeComponent
	OnInfectionEnded.Broadcast();
}

void UInfectionComponent::RestoreOriginalMaterial()
{
	USkeletalMeshComponent* Mesh = GetMeshComponent(); 
	if (!Mesh) return;

	// Get original material through Interface
	
	TArray<UMaterialInstanceDynamic*> OriginalMaterials =
		IInfectableInterface::Execute_GetOriginalDynamicMaterials(InfectableOwner);

	if (OriginalMaterials.Num() == 0)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[%s] Cannot restore material: OriginalMaterials is empty"),
			*GetOwner()->GetName());
		return;
	}

	// Restore materials
	for (int32 i = 0; i < OriginalMaterials.Num(); i++)
	{
		if (OriginalMaterials[i])
		{
			//Materials are all Dynamics
			Mesh->SetMaterial(i, OriginalMaterials[i]);
		}
	}
}

bool UInfectionComponent::ValidateInfectable()
{
    if (!InfectableOwner)
    {
        return false;
    }

    if (!InfectableOwner->Implements<UInfectableInterface>())
    {
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


