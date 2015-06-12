// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Ball.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCUP_API ABall : public APawn
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Kick")
	void addImpulseBlueprint(FVector direction, float force);
	UFUNCTION(BlueprintNativeEvent, Category = "Kickoff")
	void setKickoffState(bool state);

	virtual void setKickoffState_Implementation(bool state);
	virtual void addImpulseBlueprint_Implementation(FVector direction, float force);

	FVector getLocation();
	void addForce(FVector direction, float force);
};
