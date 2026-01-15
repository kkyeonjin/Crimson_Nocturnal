// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleWidget.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UTitleWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Enhanced Input 설정
    if (APlayerController* PlayerController = GetOwningPlayer())
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
            UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
                LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

            if (InputSubsystem && TitleContext)
            {
                InputSubsystem->AddMappingContext(TitleContext, 0);
            }
        }

        // Input Binding
        EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
        if (EnhancedInputComponent)
        {
            EnhancedInputComponent->BindAction(NavigateAction, ETriggerEvent::Triggered, this, &UTitleWidget::OnNavigate);
            EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &UTitleWidget::OnSelect);
        }
    }

    MenuCircles = { StartCircle, ContinueCircle, ExitCircle };
}

void UTitleWidget::NativeDestruct()
{
    // Mapping Context 제거
    if (APlayerController* PlayerController = GetOwningPlayer())
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
            UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
                LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

            if (InputSubsystem && TitleContext)
            {
                InputSubsystem->RemoveMappingContext(TitleContext);
            }
        }
    }

	Super::NativeDestruct();
}

void UTitleWidget::OnNavigate(const FInputActionValue& Value)
{
    const float NavigateValue = Value.Get<float>();

    UE_LOG(LogTemp, Warning, TEXT("SelectVector: %f"), NavigateValue);

    MenuCircles[TitleMenuSelectCount]->SetVisibility(ESlateVisibility::Hidden);

    TitleMenuSelectCount += NavigateValue;

    if (TitleMenuSelectCount == -1)
    {
        TitleMenuSelectCount = 2;
    }
    else if (TitleMenuSelectCount == MENUNUM)
    {
        TitleMenuSelectCount = 0;
    }

    MenuCircles[TitleMenuSelectCount]->SetVisibility(ESlateVisibility::Visible);

}

void UTitleWidget::OnSelect()
{
    if (TitleMenuSelectCount == 0)
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName("Hospital"));
    }
    else if (TitleMenuSelectCount == 1)
    {
        UE_LOG(LogTemp, Display, TEXT("Continue"));
    }
    else
    {
        FGenericPlatformMisc::RequestExit(false);
    }
}

