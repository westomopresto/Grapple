// Copyright 2020 Dmitry Karpukhin. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include "BlockoutToolsPluginSettings.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine/DataTable.h"

#include "BlockoutToolsParent.generated.h"

UENUM(BlueprintType)
enum EBlockoutMaterialType
{
	BlockoutMaterialType_Grid UMETA(DisplayName = "Blockout Grid"),
//	BlockoutMaterialType_GridPreset UMETA(DisplayName = "Blockout Grid Preset"),
	BlockoutMaterialType_CustomMaterial UMETA(DisplayName = "Custom Material")

};

USTRUCT(BlueprintType)
struct FBlockoutMaterialPreset : public FTableRowBase
{
	GENERATED_BODY()

	FBlockoutMaterialPreset() : bUseGrid(true), GridSize(100.0f), CheckerLuminance(0.35f), Color(1, 0.3, 0.05, 1), bUseTopColor(true), TopColor(0.25, 0.25, 0.25, 1) {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools", DisplayName = "Use Grid")
		bool bUseGrid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools", DisplayName = "Grid Size")
		float GridSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools", DisplayName = "Checker Luminance")
		float CheckerLuminance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools", DisplayName = "Color")
		FLinearColor Color;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools", DisplayName = "Use Top Color")
		bool bUseTopColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools", DisplayName = "Top Color")
		FLinearColor TopColor;
};

UCLASS()
class BLOCKOUTTOOLSPLUGIN_API ABlockoutToolsParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlockoutToolsParent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ABlockoutToolsParent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, Category = "Blockout Tools")
		USceneComponent* Root;

	UPROPERTY(BlueprintReadOnly, Category = "Blockout Tools")
		UBillboardComponent* Billboard;

	UPROPERTY(BlueprintReadOnly, Category = "Blockout Tools")
		UMaterialInterface* BlockoutGridParent;

	UPROPERTY(BlueprintReadOnly, Category = "Blockout Tools")
		UMaterialInstanceDynamic* BlockoutGridMID;

	UPROPERTY(BlueprintReadOnly, Category = "Blockout Tools")
		UMaterialInterface* BlockoutCurrentMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "Blockout Tools")
		TArray<UStaticMeshComponent*> BlockoutMeshComponents;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools|Blockout Material", DisplayName = "Material Type")
		TEnumAsByte<EBlockoutMaterialType> BlockoutMaterialType = BlockoutMaterialType_Grid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools|Blockout Material", DisplayName = "Color", meta = (EditCondition = "BlockoutMaterialType == EBlockoutMaterialType::BlockoutMaterialType_Grid"))
		FLinearColor BlockoutMaterialColor = FLinearColor(1.0, 0.3, 0.05, 1.0);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools|Blockout Material", DisplayName = "Use Grid", meta = (EditCondition = "BlockoutMaterialType == EBlockoutMaterialType::BlockoutMaterialType_Grid"))
		bool bBlockoutMaterialUseGrid = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools|Blockout Material", DisplayName = "Grid Size", meta = (EditCondition = "bBlockoutMaterialUseGrid && BlockoutMaterialType == EBlockoutMaterialType::BlockoutMaterialType_Grid"))
		float BlockoutMaterialGridSize = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools|Blockout Material", DisplayName = "Checker Luminance", meta = (ClampMin = "0.0", ClampMax = "1.0", EditCondition = "bBlockoutMaterialUseGrid && BlockoutMaterialType == EBlockoutMaterialType::BlockoutMaterialType_Grid"))
		float BlockoutMaterialCheckerLuminance = 0.3f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools|Blockout Material", DisplayName = "Use Top Color", meta = (EditCondition = "BlockoutMaterialType == EBlockoutMaterialType::BlockoutMaterialType_Grid"))
		bool bBlockoutMaterialUseTopColor = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools|Blockout Material", DisplayName = "Top Color", meta = (EditCondition = "bBlockoutMaterialUseTopColor && BlockoutMaterialType == EBlockoutMaterialType::BlockoutMaterialType_Grid"))
		FLinearColor BlockoutMaterialTopColor = FLinearColor(0.25, 0.25, 0.25, 1.0);

//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools|Blockout Material", DisplayName = "Blockout Material Presets", meta = (EditCondition = "BlockoutMaterialType == EBlockoutMaterialType::BlockoutMaterialType_GridPreset"))
//		FDataTableRowHandle BlockoutMaterialPresets;

	UPROPERTY(BlueprintReadOnly, Category = "Blockout Tools|Blockout Material", DisplayName = "Use Custom Material")
		bool bUseCustomMaterial = false;

	UPROPERTY(BlueprintReadOnly, Category = "Blockout Tools|Blockout Material", DisplayName = "Custom Material", meta = (EditCondition = "BlockoutMaterialType == EBlockoutMaterialType::BlockoutMaterialType_CustomMaterial"))
		UMaterialInterface* CustomMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools|Blockout Material", DisplayName = "Custom Material", meta = (EditCondition = "BlockoutMaterialType == EBlockoutMaterialType::BlockoutMaterialType_CustomMaterial"))
		UMaterialInterface* BlockoutCustomMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools|Collision", DisplayName = "Enable Collisions")
		bool bBlockoutEnableCollisions = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blockout Tools|Rendering", DisplayName = "Cast Shadows")
		bool bBlockoutCastShadows = true;

private:

	UFUNCTION(BlueprintCallable, Category = "Blockout Tools")
		void RerunConstructionScript();

	UFUNCTION(BlueprintCallable, Category = "Blockout Tools")
		void BlockoutSetMaterial();

/*	UFUNCTION(BlueprintCallable, Category = "Blockout Tools")
		void RoundVariables(bool bRoundSize, float InFloat, FVector InVector, FVector2D InVector2D); */

};