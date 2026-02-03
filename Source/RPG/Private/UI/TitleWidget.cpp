// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleWidget.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UTitleWidget::NativeConstruct()
{
    Super::NativeConstruct();

    MenuCircles = { StartCircle, ContinueCircle, ExitCircle };
}

void UTitleWidget::OnNavigate(int32 Input)
{
    MenuCircles[TitleMenuSelectCount]->SetVisibility(ESlateVisibility::Hidden);

    TitleMenuSelectCount -= Input;

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

