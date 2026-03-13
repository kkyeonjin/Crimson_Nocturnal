// ControllerMap widget in menu

#pragma once

#include "CoreMinimal.h"
#include "BaseUIInputWidget.h"
#include "ControllerMapWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnContMapClose);

UCLASS()
class RPG_API UControllerMapWidget : public UBaseUIInputWidget
{
	GENERATED_BODY()

public:
	FOnContMapClose OnContMapClose;

	void InitializeContMap();

protected:
	virtual void OnFaceButtonB() override { CloseContMap(); }

	void CloseContMap();
};
