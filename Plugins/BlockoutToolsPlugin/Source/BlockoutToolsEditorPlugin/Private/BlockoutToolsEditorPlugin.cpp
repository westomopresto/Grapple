// Copyright 2020 Dmitry Karpukhin. All Rights Reserved.

#include "BlockoutToolsEditorPlugin.h"
#include "PlacementMode/Public/IPlacementModeModule.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"

#define LOCTEXT_NAMESPACE "FBlockoutToolsEditorPluginModule"

void FBlockoutToolsEditorPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Register editor category
	int Priority = 41;
	FPlacementCategoryInfo BlockoutTools( LOCTEXT("BlockoutTools", "Blockout Tools"), "BlockoutTools", TEXT("PMBlockoutTools"), Priority);
	IPlacementModeModule::Get().RegisterPlacementCategory(BlockoutTools);

	// Register actors to category
	IPlacementModeModule::Get().RegisterPlaceableItem(BlockoutTools.UniqueHandle, MakeShareable(new FPlaceableItem(nullptr, FAssetData(Blockout_Box))));
	IPlacementModeModule::Get().RegisterPlaceableItem(BlockoutTools.UniqueHandle, MakeShareable(new FPlaceableItem(nullptr, FAssetData(Blockout_Cone))));
	IPlacementModeModule::Get().RegisterPlaceableItem(BlockoutTools.UniqueHandle, MakeShareable(new FPlaceableItem(nullptr, FAssetData(Blockout_Corner_Curved))));
	IPlacementModeModule::Get().RegisterPlaceableItem(BlockoutTools.UniqueHandle, MakeShareable(new FPlaceableItem(nullptr, FAssetData(Blockout_Cylinder))));
	IPlacementModeModule::Get().RegisterPlaceableItem(BlockoutTools.UniqueHandle, MakeShareable(new FPlaceableItem(nullptr, FAssetData(Blockout_Doorway))));
	IPlacementModeModule::Get().RegisterPlaceableItem(BlockoutTools.UniqueHandle, MakeShareable(new FPlaceableItem(nullptr, FAssetData(Blockout_Railing))));
	IPlacementModeModule::Get().RegisterPlaceableItem(BlockoutTools.UniqueHandle, MakeShareable(new FPlaceableItem(nullptr, FAssetData(Blockout_Ramp))));
	IPlacementModeModule::Get().RegisterPlaceableItem(BlockoutTools.UniqueHandle, MakeShareable(new FPlaceableItem(nullptr, FAssetData(Blockout_Skewbox))));
	IPlacementModeModule::Get().RegisterPlaceableItem(BlockoutTools.UniqueHandle, MakeShareable(new FPlaceableItem(nullptr, FAssetData(Blockout_Sphere))));
	IPlacementModeModule::Get().RegisterPlaceableItem(BlockoutTools.UniqueHandle, MakeShareable(new FPlaceableItem(nullptr, FAssetData(Blockout_Stairs_Curved))));
	IPlacementModeModule::Get().RegisterPlaceableItem(BlockoutTools.UniqueHandle, MakeShareable(new FPlaceableItem(nullptr, FAssetData(Blockout_Stairs_Linear))));
	IPlacementModeModule::Get().RegisterPlaceableItem(BlockoutTools.UniqueHandle, MakeShareable(new FPlaceableItem(nullptr, FAssetData(Blockout_Stairs_Linear_Manual))));
	IPlacementModeModule::Get().RegisterPlaceableItem(BlockoutTools.UniqueHandle, MakeShareable(new FPlaceableItem(nullptr, FAssetData(Blockout_Tube))));
	IPlacementModeModule::Get().RegisterPlaceableItem(BlockoutTools.UniqueHandle, MakeShareable(new FPlaceableItem(nullptr, FAssetData(Blockout_Window))));

	// Set Brush Image and register style
	StyleSet = MakeShareable(new FSlateStyleSet("BlockoutToolsStyle"));

	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin("BlockoutToolsPlugin")->GetBaseDir());
	FSlateImageBrush* BlockoutClassThumbnail = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/BlockoutToolsIcon64.png")), FVector2D(64.f, 64.f));
	FSlateImageBrush* BlockoutClassIcon = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/BlockoutToolsIcon64.png")), FVector2D(16.f, 16.f));

	if (BlockoutClassThumbnail && BlockoutClassIcon)
	{
		StyleSet->Set("ClassThumbnail.BlockoutToolsParent", BlockoutClassThumbnail);
		StyleSet->Set("ClassIcon.BlockoutToolsParent", BlockoutClassIcon);
		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
	}
}

void FBlockoutToolsEditorPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Unregister editor category
	if (IPlacementModeModule::IsAvailable())
	{
		IPlacementModeModule::Get().UnregisterPlacementCategory("BlockoutTools");
	}

	// Unregister style
	FSlateStyleRegistry::UnRegisterSlateStyle(StyleSet->GetStyleSetName());
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBlockoutToolsEditorPluginModule, BlockoutToolsEditorPlugin)