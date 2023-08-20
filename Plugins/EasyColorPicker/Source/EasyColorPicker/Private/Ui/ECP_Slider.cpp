// Copyright (C) Alexandre Depail. 2019. All Rights Reserved.
#include "Ui/ECP_Slider.h"
#include <math.h>
#include <algorithm>
#include "SlateMaterialBrush.h"
#include "Math/Vector2D.h"
#include "Widgets/Input/SSlider.h"
#include "Engine/Texture2D.h"

void UECP_Slider::SetGradientMaterial(UMaterial* _GradientMaterial){

  GradientMaterial = _GradientMaterial;
  this->GradientMaterialInstance = UMaterialInstanceDynamic::Create(GradientMaterial, NULL);

  this->GradientMaterialInstance->SetVectorParameterValue(FName("Color2"), this->RightColor);
  this->GradientMaterialInstance->SetVectorParameterValue(FName("Color1"), this->LeftColor);

  bNeedRebuildImage = true;

}

void UECP_Slider::SetSliderThumbMaterial(UMaterial* _SliderThumbMaterial){

  SliderThumbMaterial =_SliderThumbMaterial;
  this->SliderThumbMaterialInstance = UMaterialInstanceDynamic::Create(SliderThumbMaterial, NULL);

  FSlateMaterialBrush ThumbBrush = FSlateMaterialBrush(*(this->SliderThumbMaterialInstance), FVector2D(23, 23));
  this->WidgetStyle.SetBarThickness(12);
  this->WidgetStyle.SetNormalThumbImage(ThumbBrush);
  this->WidgetStyle.SetHoveredThumbImage(ThumbBrush);
}

void UECP_Slider::Tick(float DeltaTime){
  if(bNeedRebuildImage && this->GetCachedGeometry().Size.X != 0){
    bNeedRebuildImage = false;
    this->SetBorderRadius();
  }

  if(!bFirstCall){
    bFirstCall = true;
    OnNewColorPicked.Broadcast(this->InterpolateColor(this->GetValue()));
  }
}

bool UECP_Slider::IsTickable() const{
	return true;
}

bool UECP_Slider::IsTickableInEditor() const{
	return true;
}

bool UECP_Slider::IsTickableWhenPaused() const{
	return true;
}

TStatId UECP_Slider::GetStatId() const{
	return TStatId();
}

UWorld* UECP_Slider::GetWorld() const {
	return GetOuter()->GetWorld();
}

void UECP_Slider::SetRightColor(const FColor& NewColor){
  this->RightColor = NewColor;

  if(this->GradientMaterialInstance)
    this->GradientMaterialInstance->SetVectorParameterValue(FName("Color2"), this->RightColor);

  if(this->OnValueChanged.IsBound())
    this->OnValueChanged.Broadcast(this->GetValue());
}

void UECP_Slider::SetLeftColor(const FColor& NewColor){
  this->LeftColor = NewColor;

  if(this->GradientMaterialInstance)
    this->GradientMaterialInstance->SetVectorParameterValue(FName("Color1"), this->LeftColor);

  if(this->OnValueChanged.IsBound())
    this->OnValueChanged.Broadcast(this->GetValue());
}

void UECP_Slider::SynchronizeProperties(){
  Super::SynchronizeProperties();
  bNeedRebuildImage = true;

  this->SetLeftColor(this->LeftColor);
  this->SetRightColor(this->RightColor);
}

TSharedRef<SWidget> UECP_Slider::RebuildWidget(){
  if(bFirstRebuild){

  this->SetGradientMaterial(GradientMaterialRef.LoadSynchronous());
  this->SetSliderThumbMaterial(SliderThumbMaterialRef.LoadSynchronous());

  this->OnValueChanged.AddDynamic(this, &UECP_Slider::OnValueChangedFunction);
  bFirstRebuild = false;
  }

  bNeedRebuildImage = true;


  return Super::RebuildWidget();
}


void UECP_Slider::OnValueChangedFunction(float NewValue){

  if(this->OnNewColorPicked.IsBound())
    this->OnNewColorPicked.Broadcast(this->InterpolateColor(NewValue));
}

void GetBorderRadiusMask(int Radius, uint8* OutputMask){

  for(int y = 0; y < Radius; y++)
	for(int x = 0; x < Radius; x++ ){
	    int value = Radius - (sqrt((pow(- Radius + 0.5 + y, 2)) + pow(- Radius + 0.5 + x, 2))) + 1;

	    if(value > 1)
	        value = 255;
	    else if(value == 1)
            value = 170;
        else if(value == 0)
            value = 85;
        else
            value = 0;

	    OutputMask[x + y * Radius] = value;
	}
}


enum Corner{
  TOP_RIGHT,
  TOP_LEFT,
  BOTTOM_RIGHT,
  BOTTOM_LEFT
};

void FillCornerMask(uint8* Image, int Width, int Height, uint8* CornerMask, int Radius, Corner CornerType){


    switch(CornerType){
    case TOP_RIGHT:
        for(int y = 0; y < Radius; y++)
            for(int x = 0; x < Radius; x++)
                Image[x + y * Width] = CornerMask[x + y * Radius];
        break;

    case TOP_LEFT:
        for(int y = 0; y < Radius; y++)
            for(int x = 0; x < Radius; x++)
                Image[(Width - Radius + x) + y * Width] = CornerMask[Radius - 1 - x + y * Radius];
        break;

    case BOTTOM_RIGHT:
        for(int y = 0; y < Radius; y++)
            for(int x = 0; x < Radius; x++)
                Image[x + (Height - Radius + y) * Width] = CornerMask[x + (Radius - 1 - y) * Radius];
        break;

    case BOTTOM_LEFT:
        for(int y = 0; y < Radius; y++)
            for(int x = 0; x < Radius; x++)
                Image[(Width - Radius + x) + (Height - Radius + y) * Width] = CornerMask[Radius - 1 - x + (Radius - 1 - y) * Radius];
        break;
    }
}

void UECP_Slider::SetBorderRadius(){
  if(!this->GradientMaterialInstance)
    return;

  int TextureWidth = this->GetCachedGeometry().Size.X * 2;
  int TextureHeight = 12*2;

  UTexture2D* NewTexture = UTexture2D::CreateTransient(TextureWidth, TextureHeight, EPixelFormat::PF_R8G8B8A8);
  // NewTexture->PowerOfTwoMode = ETexturePowerOfTwoSetting::Type::None;
  NewTexture->CompressionSettings = TextureCompressionSettings::TC_EditorIcon;
  // NewTexture->AddToRoot();
  NewTexture->PlatformData = new FTexturePlatformData();
  NewTexture->PlatformData->SizeX = TextureWidth;
  NewTexture->PlatformData->SizeY = TextureHeight;
  // NewTexture->PlatformData->NumSlices = 1;
  NewTexture->PlatformData->PixelFormat = EPixelFormat::PF_R8G8B8A8;

  const int MaxRadius = std::min(static_cast<int>(floor(std::min(TextureWidth, TextureHeight) / 2)), 1000);

  // Create mask
  uint8* CornerMask = new uint8[MaxRadius * MaxRadius];
  GetBorderRadiusMask(MaxRadius, CornerMask);

  uint8* Image = new uint8[TextureWidth * TextureHeight];
  for(int i = 0; i < TextureWidth * TextureHeight; i++)
    Image[i] = 255;
  FillCornerMask(Image, TextureWidth, TextureHeight, CornerMask, MaxRadius, Corner::TOP_RIGHT);
  FillCornerMask(Image, TextureWidth, TextureHeight, CornerMask, MaxRadius, Corner::TOP_LEFT);
  FillCornerMask(Image, TextureWidth, TextureHeight, CornerMask, MaxRadius, Corner::BOTTOM_RIGHT);
  FillCornerMask(Image, TextureWidth, TextureHeight, CornerMask, MaxRadius, Corner::BOTTOM_LEFT);

  uint8* Pixels = new uint8[TextureWidth * TextureHeight * 4];
  for (int32 y = 0; y < TextureHeight; y++){
    for (int32 x = 0; x < TextureWidth; x++){
      int32 curPixelIndex = ((y * TextureWidth) + x);
      // uint8 Value = std::min(255, y);
      uint8 NewValue = Image[curPixelIndex];

      Pixels[4 * curPixelIndex] = NewValue;
      Pixels[4 * curPixelIndex + 1] = NewValue;
      Pixels[4 * curPixelIndex + 2] = NewValue;
      Pixels[4 * curPixelIndex + 3] = NewValue;
    }
  }


  // Allocate first mipmap.
  // FTexture2DMipMap* Mip = new(NewTexture->PlatformData->Mips) FTexture2DMipMap();
  NewTexture->PlatformData->Mips.Add(new FTexture2DMipMap());
  NewTexture->PlatformData->Mips[0].SizeX = TextureWidth;
  NewTexture->PlatformData->Mips[0].SizeY = TextureHeight;

  // Lock the texture so it can be modified
  NewTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
  uint8* TextureData = (uint8*) NewTexture->PlatformData->Mips[0].BulkData.Realloc(TextureWidth * TextureHeight * sizeof(uint8) * 4);
  FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * TextureHeight * TextureWidth * 4);
  NewTexture->PlatformData->Mips[0].BulkData.Unlock();

  NewTexture->UpdateResource();
  delete[] Pixels;
  delete[] CornerMask;
  delete[] Image;

  this->GradientMaterialInstance->SetTextureParameterValue(FName("AlphaTexture"), NewTexture);
  FSlateMaterialBrush ImageBrush = FSlateMaterialBrush(*(this->GradientMaterialInstance), FVector2D(TextureWidth, TextureHeight));

  this->WidgetStyle.SetNormalBarImage(ImageBrush);
  this->WidgetStyle.SetHoveredBarImage(ImageBrush);


}

FColor UECP_Slider::InterpolateColor(float InterpolationValue){
  return FColor(
    static_cast<uint8>(this->LeftColor.R + InterpolationValue * (this->RightColor.R - this->LeftColor.R)),
    static_cast<uint8>(this->LeftColor.G + InterpolationValue * (this->RightColor.G - this->LeftColor.G)),
    static_cast<uint8>(this->LeftColor.B + InterpolationValue * (this->RightColor.B - this->LeftColor.B)),
    255
  );
}
