#include "Actor/MagicCircle.h"
#include "Components/DecalComponent.h"


AMagicCircle::AMagicCircle()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AMagicCircle::InitializeDecal(UDecalComponent* MagicCircle)
{
    Decal = MagicCircle;

    Decal->SetVisibility(true);

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            DestroyTimer,
            this,
            &AMagicCircle::DestroyDecal,
            1.f,
            false
        );
    }
}

void AMagicCircle::DestroyDecal()
{
    Destroy();
}


