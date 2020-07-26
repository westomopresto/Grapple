// Copyright (C) Alexandre Depail. 2019. All Rights Reserved.
#include "Ui/ECP_ClickableArea.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Engine/Engine.h"
#include "Components/SlateWrapperTypes.h"

TSharedRef<SWidget> UECP_ClickableArea::RebuildWidget(){
  // RebuildWidget is used to get a preview in the editor.

  this->SetInputActionPriority(1);

  // Construct the RootWidget.
  UHorizontalBox* RootWidget = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("RootWidget"));
  WidgetTree->RootWidget = RootWidget;

  // Create the fake image and set to transparent.
  this->FakeImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("img"));
  this->FakeImage->SetColorAndOpacity(FColor::Transparent);

  // Set size of the image to fill the screen.
  Cast<UHorizontalBoxSlot>(RootWidget->AddChildToHorizontalBox(this->FakeImage))->SetSize(ESlateSizeRule::Type::Fill);

  return Super::RebuildWidget();
}

FReply UECP_ClickableArea::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent){
  // Execute the user function.
  if(OnMouseMoveFunction != nullptr)
    this->OnMouseMoveFunction();

  return FReply::Handled();
}

FReply UECP_ClickableArea::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent){

  // Delete widget to stop firing events.
  this->RemoveFromParent();
  GEngine->GameViewport->SetCaptureMouseOnClick(this->CaptureModeBeforeClick);

  return FReply::Handled();
}
