#include "Game/TitleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();

    FString LevelName = GetWorld()->GetMapName();

    if (LevelName.Contains(TEXT("Title")))
    {
        if (UAudioComponent* AudioComp = UGameplayStatics::SpawnSound2D(this, TitleBGM))
        {
            AudioComp->Play();
        }
    }
}
