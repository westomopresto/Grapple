// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


class USmoothSync;

class SMOOTHSYNCPLUGIN_API SmoothState
{
public:
	/// <summary>
	/// The network timestamp of the owner when the state was sent.
	/// </summary>
	float ownerTimestamp = 0;
	/// <summary>
	/// The position of the owned object when the state was sent.
	/// </summary>
	FVector position = FVector::ZeroVector;
	/// <summary>
	/// The rotation of the owned object when the state was sent.
	/// </summary>
	FQuat rotation = FQuat::Identity;
	/// <summary>
	/// The scale of the owned object when the state was sent.
	/// </summary>
	FVector scale = FVector::OneVector;
	/// <summary>
	/// The velocity of the owned object when the state was sent.
	/// </summary>
	FVector velocity = FVector::ZeroVector;
	/// <summary>
	/// The angularVelocity of the owned object when the state was sent.
	/// </summary>
	FVector angularVelocity = FVector::ZeroVector;
	/// <summary>
	/// The origin that position is relative to.
	/// </summary>
	FIntVector origin = FIntVector::ZeroValue;

	uint8 movementMode = 0;

	bool teleport = false;
	bool atPositionalRest = false;
	bool atRotationalRest = false;
	bool wasMovementModeReceived = false;

	SmoothState();
	~SmoothState();
	SmoothState(SmoothState &state);
	SmoothState(SmoothState *state);
	SmoothState(USmoothSync &smoothSyncScript);
	SmoothState(USmoothSync *smoothSyncScript);
	void Lerp(SmoothState *targetState, SmoothState *start, SmoothState *end, float t);

	void defaultTheVariables();
	void copyFromSmoothSync(USmoothSync *smoothSyncScript);
	void copyFromState(SmoothState *state);

	FVector rebasedPosition(FIntVector localWorldOrigin);
};