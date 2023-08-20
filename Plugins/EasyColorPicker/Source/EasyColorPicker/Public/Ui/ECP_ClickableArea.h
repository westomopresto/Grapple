// Copyright (C) Alexandre Depail. 2019. All Rights Reserved.
#pragma once

#include <functional>
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "Engine/EngineBaseTypes.h"
#include "ECP_ClickableArea.generated.h"

UCLASS(NotBlueprintable, NotPlaceable, HideDropdown)
class EASYCOLORPICKER_API UECP_ClickableArea : public UUserWidget {
  GENERATED_BODY()

public:
  std::function<void()> OnMouseMoveFunction;
  EMouseCaptureMode CaptureModeBeforeClick;

private:

  UPROPERTY()
  UImage* FakeImage;


  virtual TSharedRef<SWidget> RebuildWidget() override;
  virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
  virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
