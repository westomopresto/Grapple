// Copyright (C) Alexandre Depail. 2019. All Rights Reserved.
#pragma once

#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Math/Color.h"
#include "Ui/ECP_ClickableArea.h"
#include "ECP_Wheel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FValueChangedDelegate, const FColor&, NewColor);

// UCLASS(NotBlueprintable, NotPlaceable, HideDropdown)
UCLASS()
class EASYCOLORPICKER_API UECP_Wheel : public UUserWidget {
  GENERATED_BODY()

public:
  // UECP_Wheel(const FObjectInitializer & ObjectIn);
  UPROPERTY(BlueprintAssignable, Category="Widget Event")
  FValueChangedDelegate OnValueChanged;

  void SetSize(float NewSize);
  void UpdateCursorColor(const FColor& Color);
  void UpdatePreviewColor(const FColor& Color);

  void SetWheelTexture(UTexture2D* _WheelTexture);
  void SetPointerTexture(UTexture2D* _PointerTexture);
  void SetPreviewTexture(UTexture2D* _PreviewTexture);
  void SetPreviewTextureBackground(UTexture2D* _PreviewTextureBackground);
  void UpdateCursorRotation(float Angle); //Angle range: (-180, 180)
  FColor GetColorOfPixel(float X, float Y);

  UTexture2D* WheelTexture;
  UTexture2D* PointerTexture;
  UTexture2D* PreviewTexture;
  UTexture2D* PreviewTextureBackground;
private:

  UPROPERTY()
  UECP_ClickableArea* ClickableArea;
  UPROPERTY()
  UImage* WheelImage;
  UPROPERTY()
  UImage* PointerImage;
  UPROPERTY()
  UImage* PreviewImage;
  UPROPERTY()
  UImage* PreviewImageBackground;

  float ImageSize = 80;

  void SetCursorSize(float NewSize);

  virtual TSharedRef<SWidget> RebuildWidget() override;
  virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
  virtual void RemoveFromParent() override;
};
