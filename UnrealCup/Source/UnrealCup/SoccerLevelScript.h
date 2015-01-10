// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/LevelScriptActor.h"
#include "SoccerLevelScript.generated.h"

class RobotControl;

/**
 * 
 */
UCLASS()
class UNREALCUP_API ASoccerLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()
	
	
//OVERRIDES
protected:

	TArray<RobotControl*> controllerList;

	virtual void ReceiveBeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
	
};
