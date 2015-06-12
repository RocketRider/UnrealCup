// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TeamXMLParser.h"
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

public:
	UFUNCTION(BlueprintCallable, Category = "Xml")
	static FString Team1GetName();
	UFUNCTION(BlueprintCallable, Category = "Xml")
	static FString Team2GetName();
	UFUNCTION(BlueprintCallable, Category = "Xml")
	static int32 getTTP();
	UFUNCTION(BlueprintCallable, Category = "Kickoff")
	void sendKickoff(int32 team);
	UFUNCTION(BlueprintCallable, Category = "Kickoff")
	void checkPlayers();
	
//OVERRIDES
protected:

	TArray<RobotControl*> controllerList;

	virtual void ReceiveBeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
	

	void FastTick();

};
