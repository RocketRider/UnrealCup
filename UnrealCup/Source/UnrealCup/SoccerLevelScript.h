// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/LevelScriptActor.h"
#include "SoccerLevelScript.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCUP_API ASoccerLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()
	
	
//OVERRIDES
protected:

	virtual void ReceiveBeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
	
};
