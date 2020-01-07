// Copyright Weston Mitchell, LLC 2020

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TeamAIController.generated.h"

/**
 * 
 */
UCLASS()
class GRAPPLE_API ATeamAIController : public AAIController
{
GENERATED_BODY()
ATeamAIController();

public:
// Override this function 
ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
