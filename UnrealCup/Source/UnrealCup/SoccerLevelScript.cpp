// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "SoccerLevelScript.h"
#include "Robot.h"
#include "RobotControl.h"
#include "LUAScriptWorker.h"


void ASoccerLevelScript::ReceiveBeginPlay()
{
	Super::ReceiveBeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BEGIN PLAY %s"), *GetName());


	//Iterate through all Robots	
	for (TActorIterator<ARobot> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s %s"), *ActorItr->GetName(), *ActorItr->GetActorLocation().ToString());
		ARobot* robot = Cast<ARobot>(*ActorItr);
		//TODO: Replace with definition of xml file
		FString luaFile = FPaths::ConvertRelativePathToFull(FPaths::GameDir()).Append(robot->luaFile);



		RobotControl* controller = new RobotControl(robot);
		RobotWorker* worker = new LUAScriptWorker(controller, luaFile);
		controller->setWorker(worker);
		controllerList.Add(controller);
	}
	


}



void ASoccerLevelScript::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (RobotControl* controller : controllerList)
	{
		controller->End();//Deletes the worker
		delete controller;
	}

	controllerList.Empty();
}



void ASoccerLevelScript::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	for (RobotControl* controller : controllerList)
	{
		controller->Tick(DeltaSeconds);
	}


}