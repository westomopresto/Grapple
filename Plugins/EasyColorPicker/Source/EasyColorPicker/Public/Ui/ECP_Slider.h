// Copyright (C) Alexandre Depail. 2019. All Rights Reserved.
#pragma once

#include "Components/Slider.h"
#include "Math/Color.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Tickable.h"
#include "ECP_Slider.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewColorPickedDelegate, const FColor&, NewColor);

UCLASS(Abstract, NotPlaceable)
class EASYCOLORPICKER_API UECP_Slider : public USlider, public FTickableGameObject {
  GENERATED_BODY()

public:
  void SetRightColor(const FColor& NewColor);
  void SetLeftColor(const FColor& NewColor);
  void SetGradientMaterial(UMaterial* _GradientMaterial);
  void SetSliderThumbMaterial(UMaterial* _SliderThumbMaterial);

  void Tick(float DeltaTime) override;
  bool IsTickable() const override;
  bool IsTickableInEditor() const override;
  bool IsTickableWhenPaused() const override;
  TStatId GetStatId() const override;
  UWorld* GetWorld() const override;

  UPROPERTY(EditAnywhere, Category="Customization")
  FColor LeftColor;
  UPROPERTY(EditAnywhere, Category="Customization")
  FColor RightColor;
  UPROPERTY(BlueprintAssignable, Category="Widget Event")
  FOnNewColorPickedDelegate OnNewColorPicked;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Assets")
  TAssetPtr<UMaterial> GradientMaterialRef;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Assets")
  TAssetPtr<UMaterial> SliderThumbMaterialRef;

private:
  UMaterial* GradientMaterial;
  UMaterial* SliderThumbMaterial;

  bool bNeedRebuildImage = false;
  bool bFirstCall = false;
  bool bFirstRebuild = true;


  UPROPERTY()
  UMaterialInstanceDynamic* GradientMaterialInstance;
  UPROPERTY()
  UMaterialInstanceDynamic* SliderThumbMaterialInstance;

  UFUNCTION()
  void OnValueChangedFunction(float NewValue);

  void SetBorderRadius();
  FColor InterpolateColor(float InterpolationValue);

  virtual TSharedRef<SWidget> RebuildWidget() override;
  virtual void SynchronizeProperties() override;
};
