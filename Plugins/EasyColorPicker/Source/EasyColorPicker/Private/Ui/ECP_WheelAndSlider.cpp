// Copyright (C) Alexandre Depail. 2019. All Rights Reserved.
#include "Ui/ECP_WheelAndSlider.h"
#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBoxSlot.h"
#include "Types/SlateEnums.h"
#include "Math/UnrealMathUtility.h"

TSharedRef<SWidget> UECP_WheelAndSlider::RebuildWidget() {

  if(!bIsInitialized){


    // Construct the RootWidgets.
    this->HRootWidget = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HRootWidget"));
    this->WheelPicker = CreateWidget<UECP_Wheel>(GetWorld(), UECP_Wheel::StaticClass());
    // this->WheelPicker = CreateWidget<UECP_Wheel>(GetWorld(), LoadClass<UObject>(nullptr, TEXT("/EasyColorPicker/Blueprints/ECP_Slider.ECP_Slider_C")), TEXT("FirstGradientSlider"));


    this->SliderSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SliderSizeBox"));
    this->VSliderBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("VSliderBox"));
    // this->FirstGradientSlider = NewObject<UECP_Slider>(GetWorld(), SliderAsset);
    // this->SecondGradientSlider = NewObject<UECP_Slider>(GetWorld(), SliderAsset);

    this->FirstGradientSlider = WidgetTree->ConstructWidget<UECP_Slider>(LoadClass<UObject>(nullptr, TEXT("/EasyColorPicker/Blueprints/ECP_Slider.ECP_Slider_C")), TEXT("FirstGradientSlider"));
    this->SecondGradientSlider = WidgetTree->ConstructWidget<UECP_Slider>(LoadClass<UObject>(nullptr, TEXT("/EasyColorPicker/Blueprints/ECP_Slider.ECP_Slider_C")), TEXT("SecondGradientSlider"));


    // Add wheel.
    UHorizontalBoxSlot* WheelPickerSlot = Cast<UHorizontalBoxSlot>(this->HRootWidget->AddChildToHorizontalBox(this->WheelPicker));
    // WheelPickerSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
    // WheelPickerSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
    // WheelPickerSlot->SetPadding(10);

    // Add size box of sliders.
    SliderSizeBoxSlot = Cast<UHorizontalBoxSlot>(this->HRootWidget->AddChildToHorizontalBox(this->SliderSizeBox));


    // Add the vertical box to the sliders size box.
    this->SliderSizeBox->AddChild(this->VSliderBox);

    // Add the sliders.
    UVerticalBoxSlot* FirstGradientSliderSlot = Cast<UVerticalBoxSlot>(this->VSliderBox->AddChildToVerticalBox(this->FirstGradientSlider));
    FirstGradientSliderSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
    FirstGradientSliderSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
    FirstGradientSliderSlot->SetSize(ESlateSizeRule::Fill);
    UVerticalBoxSlot* SecondGradientSliderSlot = Cast<UVerticalBoxSlot>(this->VSliderBox->AddChildToVerticalBox(this->SecondGradientSlider));
    SecondGradientSliderSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
    SecondGradientSliderSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
    SecondGradientSliderSlot->SetSize(ESlateSizeRule::Fill);





    // Set the root widget of this widget.
    WidgetTree->RootWidget = this->HRootWidget;

    bIsInitialized = true;
  }
  SliderSizeBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
  SliderSizeBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);

  return Super::RebuildWidget();
}


void UECP_WheelAndSlider::NativeOnInitialized() {



  Super::NativeOnInitialized();

}

void UECP_WheelAndSlider::OnWidgetRebuilt(){



  if(!bIsInitialized || !this->WheelPicker){
    Super::OnWidgetRebuilt();
    return;
  }else{
    // // Set all textures.
    this->WheelPicker->SetWheelTexture(WheelTextureRef.LoadSynchronous());
    this->WheelPicker->SetPointerTexture(PointerTextureRef.LoadSynchronous());
    this->WheelPicker->SetPreviewTexture(PreviewTextureRef.LoadSynchronous());
    this->WheelPicker->SetPreviewTextureBackground(PreviewTextureBackgroundRef.LoadSynchronous());

    // Set default values.
    this->WheelPicker->SetSize(this->WheelSize);
    this->UpdateSliderWidth();
    this->UpdateSliderHeight();

    if(!alreadySet){

      this->WheelPicker->OnValueChanged.AddDynamic(this, &UECP_WheelAndSlider::OnWheelPickerValueChanged);
      this->FirstGradientSlider->OnNewColorPicked.AddDynamic(this, &UECP_WheelAndSlider::OnFirstSliderValueChanged);
      this->SecondGradientSlider->OnNewColorPicked.AddDynamic(this, &UECP_WheelAndSlider::OnSecondSliderValueChanged);
      alreadySet = true;
    }

  this->FirstGradientSlider->SetValue(1);
  this->FirstGradientSlider->SetRightColor(FColor::Red);
  this->FirstGradientSlider->SetLeftColor(FColor::White);
  this->SecondGradientSlider->SetRightColor(FColor::Red);
  this->SecondGradientSlider->SetLeftColor(FColor::Black);



  Super::OnWidgetRebuilt();
}
}

#if WITH_EDITOR
void UECP_WheelAndSlider::PostEditChangeProperty(struct FPropertyChangedEvent& e){
  Super::PostEditChangeProperty(e);

  // Get the PropertyName.
  FName PropertyName = e.GetPropertyName();

  if (PropertyName == GET_MEMBER_NAME_CHECKED(UECP_WheelAndSlider, WheelSize)) {
    if(this->WheelPicker != nullptr)
      this->WheelPicker->SetSize(WheelSize);
  } else if (PropertyName == GET_MEMBER_NAME_CHECKED(UECP_WheelAndSlider, SliderWidth)) {
    this->UpdateSliderWidth();
  } else if (PropertyName == GET_MEMBER_NAME_CHECKED(UECP_WheelAndSlider, SliderHeight)) {
    this->UpdateSliderHeight();
  }
}
#endif

void UECP_WheelAndSlider::UpdateSliderWidth(){
  if(!this->SliderSizeBox)
  return;

  this->SliderSizeBox->SetWidthOverride(this->SliderWidth);
}

void UECP_WheelAndSlider::UpdateSliderHeight(){
  if(!this->SliderSizeBox)
    return;

  this->SliderSizeBox->SetHeightOverride(this->SliderHeight);
}


void UECP_WheelAndSlider::OnWheelPickerValueChanged(const FColor& NewColor){

  this->FirstGradientSlider->SetRightColor(NewColor);
}

void UECP_WheelAndSlider::OnFirstSliderValueChanged(const FColor& NewColor){

  this->SecondGradientSlider->SetRightColor(NewColor);
}

void UECP_WheelAndSlider::OnSecondSliderValueChanged(const FColor& NewColor){

  this->WheelPicker->UpdatePreviewColor(NewColor);

  ColorToGet = NewColor;

  if(this->OnNewColorPicked.IsBound())
    this->OnNewColorPicked.Broadcast(NewColor);
}

FColor UECP_WheelAndSlider::GetColor(){
  return ColorToGet;
}

void UECP_WheelAndSlider::SetColor(const FColor& NewColor){

    FLinearColor NewLinearColor = FLinearColor(NewColor.R / 255.0f, NewColor.G / 255.0f, NewColor.B / 255.0f);
    FLinearColor HSVLinearColor = NewLinearColor.LinearRGBToHSV();

    float Angle = 360 - HSVLinearColor.R;
    this->WheelPicker->UpdateCursorRotation(Angle);
    this->FirstGradientSlider->SetValue(HSVLinearColor.G);
    this->SecondGradientSlider->SetValue(HSVLinearColor.B);

    HSVLinearColor.G = 1;
    HSVLinearColor.B = 1;
    this->OnWheelPickerValueChanged(HSVLinearColor.HSVToLinearRGB().ToFColor(false));
}
