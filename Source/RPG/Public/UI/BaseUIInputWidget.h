// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUIInputWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBaseUIInputWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    // 패드 입력 처리 (모든 UI가 상속해서 사용)
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

protected:
    // 서브클래스에서 오버라이드할 함수들 (virtual)

    // D-Pad 입력
    virtual void OnDPadUp() {}
    virtual void OnDPadDown() {}
    virtual void OnDPadLeft() {}
    virtual void OnDPadRight() {}

    // Face 버튼 입력
    virtual void OnFaceButtonA() {}  // A 버튼
    virtual void OnFaceButtonB() {}  // B 버튼
    virtual void OnFaceButtonX() {}  // X 버튼
    virtual void OnFaceButtonY() {}  // Y 버튼

    // 어깨 버튼
    virtual void OnShoulderButtonLB() {}
    virtual void OnShoulderButtonRB() {}

    // 로그 (디버그용)
    void LogInput(const FString& InputName);
};

