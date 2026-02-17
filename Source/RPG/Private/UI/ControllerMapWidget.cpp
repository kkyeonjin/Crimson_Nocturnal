#include "UI/ControllerMapWidget.h"

void UControllerMapWidget::InitializeContMap()
{
	this->SetVisibility(ESlateVisibility::Visible);
	this->SetKeyboardFocus();
}

void UControllerMapWidget::CloseContMap()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}
