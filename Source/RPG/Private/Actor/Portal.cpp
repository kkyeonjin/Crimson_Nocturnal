#include "Actor/Portal.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/RPGGameMode.h"

APortal::APortal()
{
    PrimaryActorTick.bCanEverTick = false;
}

void APortal::OnInteract()
{
    if (!DestinationMap.IsNone())
    {
        FString LevelString = DestinationMap.ToString();

        ARPGGameMode* GM = Cast<ARPGGameMode>(GetWorld()->GetAuthGameMode());

        // RuntimeDataSave is required cause Player+Components are newly loaded when new level is open
        GM->OnSaveRequired.Broadcast();

        // Move to saloon portal's entrance when exit from the saloon
        if (ExitingSaloon)
        {
            LevelString += TEXT("#SaloonDoor");
        }

        UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelString));

    }
}

