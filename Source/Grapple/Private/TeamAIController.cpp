// Copyright Weston Mitchell, LLC 2020


#include "TeamAIController.h"

ATeamAIController::ATeamAIController()
{
	SetGenericTeamId(FGenericTeamId(5));
}

ETeamAttitude::Type ATeamAIController::GetTeamAttitudeTowards(const AActor& Other) const
{

  if (const APawn* OtherPawn = Cast<APawn>(&Other)) {

    if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
    {
      return Super::GetTeamAttitudeTowards(*OtherPawn->GetController());
    }
  }

  return ETeamAttitude::Neutral;
}