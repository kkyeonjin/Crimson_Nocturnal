// ControllerMap widget in menu

#pragma once

#include "CoreMinimal.h"
#include "BaseUIInputWidget.h"
#include "ControllerMapWidget.generated.h"

UCLASS()
class RPG_API UControllerMapWidget : public UBaseUIInputWidget
{
	GENERATED_BODY()

public:
	void InitializeContMap();

protected:
	virtual void OnFaceButtonB() override { CloseContMap(); }

	void CloseContMap();
};
