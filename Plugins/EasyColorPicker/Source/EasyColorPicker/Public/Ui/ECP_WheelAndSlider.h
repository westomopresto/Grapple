// Copyright (C) Alexandre Depail. 2019. All Rights Reserved.
#pragma once

#include "Blueprint/UserWidget.h"
#include "Ui/ECP_Wheel.h"
#include "Ui/ECP_Slider.h"
#include "Components/SizeBox.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/SizeBoxSlot.h"
#include "UObject/SoftObjectPtr.h"
#include "Components/HorizontalBoxSlot.h"
#include "Engine/Texture2D.h"
#include "Math/Color.h"
#include "ECP_WheelAndSlider.generated.h"

UENUM(BlueprintType)
enum class ESliderOrientation : uint8 {
  Horizontal,
  Vertical
};

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewColorPickedDelegate, const FColor&, NewColor);

UCLASS()
class EASYCOLORPICKER_API UECP_WheelAndSlider : public UUserWidget {
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, Category="Customization", meta=(ClampMin = "80", ClampMax = "400.0", UIMin = "80.0", UIMax = "400.0"))
  float WheelSize = 120;
  UPROPERTY(EditAnywhere, Category="Customization", meta=(ClampMin = "80", ClampMax = "400.0", UIMin = "80.0", UIMax = "400.0"))
  float SliderWidth = 220;
  UPROPERTY(EditAnywhere, Category="Customization", meta=(ClampMin = "80", ClampMax = "400.0", UIMin = "80.0", UIMax = "400.0"))
  float SliderHeight = 100;

  UPROPERTY(BlueprintAssignable, Category="Widget Event")
  FOnNewColorPickedDelegate OnNewColorPicked;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Assets")
  TAssetPtr<UTexture2D> WheelTextureRef;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Assets")
  TAssetPtr<UTexture2D> PointerTextureRef;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Assets")
  TAssetPtr<UTexture2D> PreviewTextureRef;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Assets")
  TAssetPtr<UTexture2D> PreviewTextureBackgroundRef;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Assets")
  TAssetPtr<UMaterial> GradientMaterialRef;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Assets")
  TAssetPtr<UMaterial> SliderThumbMaterialRef;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Assets")
  TSubclassOf<class UECP_Slider> SliderAsset;

  UPROPERTY()
  UECP_Wheel* WheelPicker;
  UPROPERTY()
  UECP_Slider* FirstGradientSlider;
  UPROPERTY()
  UECP_Slider* SecondGradientSlider;
  UPROPERTY()
  USizeBox* SliderSizeBox;
  UPROPERTY()
  UHorizontalBox* HRootWidget;
  UPROPERTY()
  UVerticalBox* VSliderBox;
  UPROPERTY()
  USizeBoxSlot* FirstBoxSlot;
  UPROPERTY()
  USizeBoxSlot* SecondBoxSlot;

  UFUNCTION(BlueprintCallable, Category="Customization")
  void SetColor(const FColor& NewColor);
  UFUNCTION(BlueprintCallable, Category="Customization")
  FColor GetColor();
private:
  bool bIsInitialized = false;
  bool alreadySet = false;
  virtual TSharedRef<SWidget> RebuildWidget() override;
  virtual void NativeOnInitialized() override;
  virtual void OnWidgetRebuilt() override;
  FColor ColorToGet;

  UHorizontalBoxSlot* SliderSizeBoxSlot;


#if WITH_EDITOR
  virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e);
#endif

  void UpdateSliderWidth();
  void UpdateSliderHeight();

  UFUNCTION()
  void OnWheelPickerValueChanged(const FColor& NewColor);
  UFUNCTION()
  void OnFirstSliderValueChanged(const FColor& NewColor);
  UFUNCTION()
  void OnSecondSliderValueChanged(const FColor& NewColor);

};
