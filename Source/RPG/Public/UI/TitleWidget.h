// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define MENUNUM  3

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidget.generated.h"

class UInputAction;
struct FInputActionValue;
class UImage;
class UEnhancedInputComponent;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;

/**
 * 
 */
UCLASS()
class RPG_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override; //BeginPlay같은거, 초기세팅 
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	UImage* StartCircle;

	UPROPERTY(meta = (BindWidget))
	UImage* ContinueCircle;

	UPROPERTY(meta = (BindWidget))
	UImage* ExitCircle;

private:
	UEnhancedInputComponent* EnhancedInputComponent;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* TitleContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* NavigateAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SelectAction;

	int32 TitleMenuSelectCount = 0;

	TArray<UImage*> MenuCircles;

	void OnNavigate(const FInputActionValue& Value);
	void OnSelect();
};
