#include "UI/BaseUIInputWidget.h"

void UBaseUIInputWidget::NativeConstruct()
{
    Super::NativeConstruct();
    SetIsFocusable(true);
    UE_LOG(LogTemp, Warning, TEXT("BaseUIInputWidget Constructed - Focusable: %d"), IsFocusable());
}

FReply UBaseUIInputWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    const FKey Key = InKeyEvent.GetKey();

    // D-Pad
    if (Key == EKeys::Gamepad_DPad_Up)
    {
        LogInput(TEXT("DPad Up"));
        OnDPadUp();
        return FReply::Handled();
    }

    if (Key == EKeys::Gamepad_DPad_Down)
    {
        LogInput(TEXT("DPad Down"));
        OnDPadDown();
        return FReply::Handled();
    }

    if (Key == EKeys::Gamepad_DPad_Left)
    {
        LogInput(TEXT("DPad Left"));
        OnDPadLeft();
        return FReply::Handled();
    }

    if (Key == EKeys::Gamepad_DPad_Right)
    {
        LogInput(TEXT("DPad Right"));
        OnDPadRight();
        return FReply::Handled();
    }

    // Face Buttons
    if (Key == EKeys::Gamepad_FaceButton_Bottom)  // A
    {
        LogInput(TEXT("Button A"));
        OnFaceButtonA();
        return FReply::Handled();
    }

    if (Key == EKeys::Gamepad_FaceButton_Right)  // B
    {
        LogInput(TEXT("Button B"));
        OnFaceButtonB();
        return FReply::Handled();
    }

    if (Key == EKeys::Gamepad_FaceButton_Left)  // X
    {
        LogInput(TEXT("Button X"));
        OnFaceButtonX();
        return FReply::Handled();
    }

    if (Key == EKeys::Gamepad_FaceButton_Top)  // Y
    {
        LogInput(TEXT("Button Y"));
        OnFaceButtonY();
        return FReply::Handled();
    }

    // Shoulder Buttons
    if (Key == EKeys::Gamepad_LeftShoulder)
    {
        LogInput(TEXT("LB"));
        OnShoulderButtonLB();
        return FReply::Handled();
    }

    if (Key == EKeys::Gamepad_RightShoulder)
    {
        LogInput(TEXT("RB"));
        OnShoulderButtonRB();
        return FReply::Handled();
    }

    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UBaseUIInputWidget::LogInput(const FString& InputName)
{
    UE_LOG(LogTemp, Warning, TEXT("Input: %s"), *InputName);
}

