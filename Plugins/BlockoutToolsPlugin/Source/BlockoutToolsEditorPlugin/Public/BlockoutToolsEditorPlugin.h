// Copyright 2020 Dmitry Karpukhin. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Templates/SubclassOf.h"
#include "Styling/SlateStyle.h"
#include "BlockoutToolsParent.h"

class FBlockoutToolsEditorPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	TSharedPtr<FSlateStyleSet> StyleSet;

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite)
	TSubclassOf<ABlockoutToolsParent> Blockout_Box = LoadClass<ABlockoutToolsParent>(nullptr, TEXT("Blueprint'/BlockoutToolsPlugin/Blueprints/Blockout_Box.Blockout_Box_C'"));

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite)
	TSubclassOf<ABlockoutToolsParent> Blockout_Cone = LoadClass<ABlockoutToolsParent>(nullptr, TEXT("Blueprint'/BlockoutToolsPlugin/Blueprints/Blockout_Cone.Blockout_Cone_C'"));

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite)
	TSubclassOf<ABlockoutToolsParent> Blockout_Corner_Curved = LoadClass<ABlockoutToolsParent>(nullptr, TEXT("Blueprint'/BlockoutToolsPlugin/Blueprints/Blockout_Corner_Curved.Blockout_Corner_Curved_C'"));

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite)
	TSubclassOf<ABlockoutToolsParent> Blockout_Cylinder = LoadClass<ABlockoutToolsParent>(nullptr, TEXT("Blueprint'/BlockoutToolsPlugin/Blueprints/Blockout_Cylinder.Blockout_Cylinder_C'"));

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite)
	TSubclassOf<ABlockoutToolsParent> Blockout_Doorway = LoadClass<ABlockoutToolsParent>(nullptr, TEXT("Blueprint'/BlockoutToolsPlugin/Blueprints/Blockout_Doorway.Blockout_Doorway_C'"));

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite)
	TSubclassOf<ABlockoutToolsParent> Blockout_Railing = LoadClass<ABlockoutToolsParent>(nullptr, TEXT("Blueprint'/BlockoutToolsPlugin/Blueprints/Blockout_Railing.Blockout_Railing_C'"));

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite)
	TSubclassOf<ABlockoutToolsParent> Blockout_Ramp = LoadClass<ABlockoutToolsParent>(nullptr, TEXT("Blueprint'/BlockoutToolsPlugin/Blueprints/Blockout_Ramp.Blockout_Ramp_C'"));

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite)
	TSubclassOf<ABlockoutToolsParent> Blockout_Skewbox = LoadClass<ABlockoutToolsParent>(nullptr, TEXT("Blueprint'/BlockoutToolsPlugin/Blueprints/Blockout_Skewbox.Blockout_Skewbox_C'"));

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite)
	TSubclassOf<ABlockoutToolsParent> Blockout_Sphere = LoadClass<ABlockoutToolsParent>(nullptr, TEXT("Blueprint'/BlockoutToolsPlugin/Blueprints/Blockout_Sphere.Blockout_Sphere_C'"));

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite)
	TSubclassOf<ABlockoutToolsParent> Blockout_Stairs_Curved = LoadClass<ABlockoutToolsParent>(nullptr, TEXT("Blueprint'/BlockoutToolsPlugin/Blueprints/Blockout_Stairs_Curved.Blockout_Stairs_Curved_C'"));

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite)
	TSubclassOf<ABlockoutToolsParent> Blockout_Stairs_Linear = LoadClass<ABlockoutToolsParent>(nullptr, TEXT("Blueprint'/BlockoutToolsPlugin/Blueprints/Blockout_Stairs_Linear.Blockout_Stairs_Linear_C'"));

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite)
	TSubclassOf<ABlockoutToolsParent> Blockout_Stairs_Linear_Manual = LoadClass<ABlockoutToolsParent>(nullptr, TEXT("Blueprint'/BlockoutToolsPlugin/Blueprints/Blockout_Stairs_Linear_Manual.Blockout_Stairs_Linear_Manual_C'"));

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite)
	TSubclassOf<ABlockoutToolsParent> Blockout_Tube = LoadClass<ABlockoutToolsParent>(nullptr, TEXT("Blueprint'/BlockoutToolsPlugin/Blueprints/Blockout_Tube.Blockout_Tube_C'"));

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite)
	TSubclassOf<ABlockoutToolsParent> Blockout_Window = LoadClass<ABlockoutToolsParent>(nullptr, TEXT("Blueprint'/BlockoutToolsPlugin/Blueprints/Blockout_Window.Blockout_Window_C'"));
};