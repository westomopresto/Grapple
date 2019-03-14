// Fill out your copyright notice in the Description page of Project Settings.

#include "Character_w_Tags.h"

// Sets default values
ACharacter_w_Tags::ACharacter_w_Tags()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacter_w_Tags::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacter_w_Tags::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacter_w_Tags::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

