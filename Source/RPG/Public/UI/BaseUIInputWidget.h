// Base class that is inherited by widget classes
// Enable process input without player action (when InputMode = UI Only)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUIInputWidget.generated.h"

UCLASS()
class RPG_API UBaseUIInputWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    // Manage game pad input
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

protected:
    // D-Pad 
    virtual void OnDPadUp() {}
    virtual void OnDPadDown() {}
    virtual void OnDPadLeft() {}
    virtual void OnDPadRight() {}

    // Face Button
    virtual void OnFaceButtonA() {}  // A 
    virtual void OnFaceButtonB() {}  // B 
    virtual void OnFaceButtonX() {}  // X 
    virtual void OnFaceButtonY() {}  // Y 

    virtual void OnShoulderButtonLB() {}
    virtual void OnShoulderButtonRB() {}

    void LogInput(const FString& InputName);
};

