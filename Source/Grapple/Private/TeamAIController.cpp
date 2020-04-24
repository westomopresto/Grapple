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

void ATeamAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		FRotator NewControlRotation = GetControlRotation();

		// Look toward focus
		const FVector FocalPoint = GetFocalPoint();
		if (FAISystem::IsValidLocation(FocalPoint))
		{
			NewControlRotation = (FocalPoint - MyPawn->GetPawnViewLocation()).Rotation();
		}
		else if (bSetControlRotationFromPawnOrientation)
		{
			NewControlRotation = MyPawn->GetActorRotation();
		}

		// Don't pitch view unless looking at another pawn
		//if (NewControlRotation.Pitch != 0 && Cast<APawn>(GetFocusActor()) == nullptr)
		//{
		//	NewControlRotation.Pitch = 0.f;
		//}

		SetControlRotation(NewControlRotation);

		if (bUpdatePawn)
		{
			const FRotator CurrentPawnRotation = MyPawn->GetActorRotation();

			if (CurrentPawnRotation.Equals(NewControlRotation, 1e-3f) == false)
			{
				MyPawn->FaceRotation(NewControlRotation, DeltaTime);
			}
		}
	}
}