// Ending widget appears when game ends (after killing boss)

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUIInputWidget.h"
#include "EndingWidget.generated.h"

UCLASS()
class RPG_API UEndingWidget : public UBaseUIInputWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	virtual void OnFaceButtonB() override { EndGame(); }

private:
	void EndGame();
	
};
