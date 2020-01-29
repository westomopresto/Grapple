// Copyright Weston Mitchell, LLC 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "HitBoxCapsule.generated.h"

class UHitBoxCapsule;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GRAPPLE_API UHitBoxCapsule : public UCapsuleComponent
{
	GENERATED_BODY()

public:
	UHitBoxCapsule();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBoxName")
		FString HitBoxName;
	
};
