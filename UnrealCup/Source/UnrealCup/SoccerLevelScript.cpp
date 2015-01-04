// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "SoccerLevelScript.h"
#include "Robot.h"
#include "RobotControl.h"
#include "RobotWorker.h"


void ASoccerLevelScript::ReceiveBeginPlay()
{
	Super::ReceiveBeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BEGIN PLAY %s"), *GetName());


	//Iterate through all Robots	
	for (TActorIterator<ARobot> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s %s"), *ActorItr->GetName(), *ActorItr->GetActorLocation().ToString());
		RobotWorker* worker = NULL;//new RobotWorker();
		RobotControl* controller = new RobotControl(Cast<ARobot>(*ActorItr), worker);
		//TODO: Add to list
	}
	


}



void ASoccerLevelScript::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);


}



void ASoccerLevelScript::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


}