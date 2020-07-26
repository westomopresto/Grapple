// Copyright (C) Alexandre Depail. 2019. All Rights Reserved.
#include "Ui/ECP_Wheel.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SlateWrapperTypes.h"
#include "Input/Reply.h"
#include "Components/Overlay.h"
#include "TextureResource.h"
#include "Serialization/BulkData.h"
#include "Engine/Engine.h"
#include "Slate/WidgetTransform.h"
#include <math.h>

void UECP_Wheel::SetWheelTexture(UTexture2D* _WheelTexture){

  WheelTexture = _WheelTexture;

  // Construct the wheel image from texture.
  this->WheelImage->SetBrushFromTexture(WheelTexture, true);
}

void UECP_Wheel::SetPointerTexture(UTexture2D* _PointerTexture){

  PointerTexture = _PointerTexture;

  // Construct the wheel cursor from texture.
  this->PointerImage->SetBrushFromTexture(PointerTexture, true);
  this->PointerImage->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UECP_Wheel::SetPreviewTexture(UTexture2D* _PreviewTexture){

  PreviewTexture = _PreviewTexture;

  // Construct the wheel preview from texture.
  this->PreviewImage->SetBrushFromTexture(PreviewTexture, true);
  this->PreviewImage->SetVisibility(ESlateVisibility::HitTestInvisible);

}
void UECP_Wheel::SetPreviewTextureBackground(UTexture2D* _PreviewTextureBackground){

  PreviewTextureBackground = _PreviewTextureBackground;

  this->PreviewImageBackground->SetBrushFromTexture(PreviewTextureBackground, true);
  this->PreviewImageBackground->SetVisibility(ESlateVisibility::HitTestInvisible);
}


TSharedRef<SWidget> UECP_Wheel::RebuildWidget(){
  this->WheelImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
  this->PointerImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
  this->PreviewImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
  this->PreviewImageBackground = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());


  // Get the texture once for all futur instances.
  // WheelTexture = WheelTextureRef.LoadSynchronous();
  // PointerTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/EasyColorPicker/Textures/PointerTexture.PointerTexture")));
  // PreviewTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/EasyColorPicker/Textures/PreviewTexture.PreviewTexture")));
  // PreviewTextureBackground = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/EasyColorPicker/Textures/PreviewTextureBackground.PreviewTextureBackground")));

  this->ClickableArea = CreateWidget<UECP_ClickableArea>(GetWorld(), UECP_ClickableArea::StaticClass());
  this->ClickableArea->OnMouseMoveFunction = [this]() {
    // Get the widget position.
    FVector2D _, WidgetAbsolutePosition;
    FVector2D MouseAbsolutePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
    USlateBlueprintLibrary::LocalToViewport(
      this,
      GetCachedGeometry(),
      FVector2D(0,0),
      _,
      WidgetAbsolutePosition
    );

    // Calculate the mouse position in the widget (negative values exits, the
    // 0,0 coord is the center of the wheel image.
    // -1;-1 = top left corner
    // 1;1 = bottom right corner
    FVector2D MousePositionInWidget(
      MouseAbsolutePosition.X - WidgetAbsolutePosition.X - this->ImageSize / 2,
      MouseAbsolutePosition.Y - WidgetAbsolutePosition.Y - this->ImageSize / 2
    );
    MousePositionInWidget.Normalize();

    // Get the color of the pixel targeted.
    FColor NewColor = this->GetColorOfPixel(
      MousePositionInWidget.X,
      MousePositionInWidget.Y
    );
    // Workaround to able the user to pick a color inside the ring (in the transparent part).
    NewColor.A = 255;


    float NewAngle = FMath::Acos(FVector2D::DotProduct(MousePositionInWidget, FVector2D(0,-1)));
    NewAngle = FMath::RadiansToDegrees(NewAngle);

    // Add the negative sign if needed.
    if(MousePositionInWidget.X < 0)
      NewAngle = -NewAngle;

    // Update the rotation of the preview.
    this->UpdateCursorRotation(NewAngle);
    this->OnValueChanged.Broadcast(NewColor);
  };

  // Construct the RootWidget.
  UOverlay* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
  WidgetTree->RootWidget = RootWidget;


  // this->WheelCursor = CreateWidget<UWheelCursor>(GetWorld(), UWheelCursor::StaticClass());

  // ----
  // Construct the RootWidget.
  UOverlay* Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("UOverlay"));

  Overlay->SetVisibility(ESlateVisibility::HitTestInvisible);





  // Apply default colors.
  this->UpdatePreviewColor(FColor::Black);

  Overlay->AddChildToOverlay(this->PreviewImageBackground);
  Overlay->AddChildToOverlay(this->PointerImage);
  Overlay->AddChildToOverlay(this->PreviewImage);
  // ----

  RootWidget->AddChildToOverlay(this->WheelImage);
  // RootWidget->AddChildToOverlay(this->WheelCursor);
  RootWidget->AddChildToOverlay(Overlay);


  return Super::RebuildWidget();
}

void UECP_Wheel::SetCursorSize(float NewSize){
  this->PointerImage->SetBrushSize(FVector2D(NewSize,NewSize));
  this->PreviewImage->SetBrushSize(FVector2D(NewSize,NewSize));
  this->PreviewImageBackground->SetBrushSize(FVector2D(NewSize,NewSize));
}

void UECP_Wheel::UpdateCursorColor(const FColor& Color){
  this->PointerImage->SetColorAndOpacity(Color);
}

void UECP_Wheel::UpdatePreviewColor(const FColor& Color){
  this->PreviewImage->SetColorAndOpacity(Color);
}

void UECP_Wheel::UpdateCursorRotation(float Angle){
  FWidgetTransform NewTransform;
  NewTransform.Angle = Angle;
  this->PointerImage->SetRenderTransform(NewTransform);
}


void UECP_Wheel::SetSize(float NewSize){
  this->ImageSize = NewSize;

  if(this->WheelImage != nullptr)
    this->WheelImage->SetBrushSize(FVector2D(NewSize, NewSize));
  if(this->PointerImage && this->PreviewImage && this->PreviewImageBackground)
    this->SetCursorSize(NewSize);
}

FReply UECP_Wheel::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent){


  this->ClickableArea->CaptureModeBeforeClick = GEngine->GameViewport->CaptureMouseOnClick();
  GEngine->GameViewport->SetCaptureMouseOnClick(EMouseCaptureMode::NoCapture);

  this->ClickableArea->AddToViewport(9999999);

  return FReply::Handled();

}

FColor UECP_Wheel::GetColorOfPixel(float X, float Y){

  float NewAngle = FMath::Acos(FVector2D::DotProduct(FVector2D(X, Y), FVector2D(0,-1)));
  NewAngle = FMath::RadiansToDegrees(NewAngle);

  // Add the negative sign if needed.
  if(X < 0)
    NewAngle = 360 - NewAngle;

  NewAngle = 360 - NewAngle;


  const FColor PixelColor = FLinearColor::MakeFromHSV8(NewAngle / 360.0f * 255.0f, 255, 255).ToFColor(false);

  return PixelColor;
}

void UECP_Wheel::RemoveFromParent(){
  if(this->ClickableArea)
    this->ClickableArea->RemoveFromParent();

  Super::RemoveFromParent();
}
