// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Character_w_Tags.generated.h"

class ACharacter_w_Tags;

UCLASS()
class GRAPPLE_API ACharacter_w_Tags : public ACharacter, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:	
	ACharacter_w_Tags();

	// Called when the game starts or spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Falling() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = GameplayTags; return; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
	FGenericTeamId TeamId;

	//teams
private:
	FGenericTeamId GetGenericTeamId() const;
};
