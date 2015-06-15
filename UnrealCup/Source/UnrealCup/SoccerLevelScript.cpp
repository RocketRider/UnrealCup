// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "SoccerLevelScript.h"
#include "Robot.h"
#include "RobotControl.h"
#include "LUAScriptWorker.h"
#include "Ball.h"

FString ASoccerLevelScript::Team1GetName()
{
	TeamXMLParser* p = new TeamXMLParser();
	return p->GetNameTeam1XML();
}

FString ASoccerLevelScript::Team2GetName()
{
	TeamXMLParser* p = new TeamXMLParser();
	return p->GetNameTeam2XML();
}

int32 ASoccerLevelScript::getTTP()
{
	TeamXMLParser* p = new TeamXMLParser();
	return p->getTimeToPlay();
}

void ASoccerLevelScript::sendKickoff(int32 team)
{
	if (team < -1 || team > 2) return;

	//Iterate through all Robots	
	for (TActorIterator<ARobot> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ARobot* robot = Cast<ARobot>(*ActorItr);
		robot->receiveKickoffTeam(team);
	}
	for (TActorIterator<ABall> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		Cast<ABall>(*ActorItr)->setKickoffState(team != -1);
	}
}

void ASoccerLevelScript::checkPlayers()
{
	for (TActorIterator<ARobot> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ARobot* robot = Cast<ARobot>(*ActorItr);

		FVector position = robot->getPosition();
		int teamNumber = robot->getTeamId();
		int xPos = position.X;
		if (teamNumber == 1) xPos = -position.X;
		if (position.X > 0) robot->GetController()->EndPlay(EEndPlayReason::RemovedFromWorld);
	}
}

void ASoccerLevelScript::ReceiveBeginPlay()
{
	Super::ReceiveBeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BEGIN PLAY %s"), *GetName());

	kickoffState = 0; // TODO Setzen?
	ABall* ball = nullptr;
	for (TActorIterator<ABall> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ball = Cast<ABall>(*ActorItr);
		//UE_LOG(LogTemp, Error, TEXT("%s %s"), *ball->GetName(), *ball->GetActorLocation().ToString());
		break;
	}

	AActor* goal1 = nullptr;
	AActor* goal2 = nullptr;
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{

		//TODO: Improve finding goals! These name change if the goal is modified!
		if (ActorItr->GetName().Equals("football_goal_C_0") || ActorItr->GetName().Equals("football_goal_C_2"))
		{
			goal1 = Cast<AActor>(*ActorItr);
		}
		if (ActorItr->GetName().Equals("football_goal_C_1") || ActorItr->GetName().Equals("football_goal_C_3"))
		{
			goal2 = Cast<AActor>(*ActorItr);
		}
	}


	//Iterate through all Robots	
	for (TActorIterator<ARobot> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		//UE_LOG(LogTemp, Error, TEXT("%s %s"), *ActorItr->GetName(), *ActorItr->GetActorLocation().ToString());
		ARobot* robot = Cast<ARobot>(*ActorItr);
		//TODO: Replace with definition of xml file
		FString luaFile = FPaths::ConvertRelativePathToFull(FPaths::GameDir()).Append(robot->luaFile);

		robot->setBall(ball);
		robot->setGoals(goal1, goal2);

		RobotControl* controller = new RobotControl(robot);
		RobotWorker* worker = new LUAScriptWorker(controller, luaFile);
		controller->setWorker(worker);
		controllerList.Add(controller);
	}
	sendKickoff(kickoffState);




	//Fast Tick
	//TODO: Update this function:
	GetWorldTimerManager().SetTimer(this, &ASoccerLevelScript::FastTick, 0.0001f, true);

}



void ASoccerLevelScript::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearAllTimersForObject(this);


	for (RobotControl* controller : controllerList)
	{
		controller->End();//Deletes the worker
		delete controller;
	}

	controllerList.Empty();
}


//Called onece per frame
void ASoccerLevelScript::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Use Fast Tick for more ticks per secound
	/*
	for (RobotControl* controller : controllerList)
	{
		controller->Tick(DeltaSeconds);
	}
	*/


}


void ASoccerLevelScript::FastTick()
{
	//Not perfect but should be good enouth (is the wanted rate, not the actual time between two ticks)
	float DeltaSeconds = GetWorldTimerManager().GetTimerRate(this, &ASoccerLevelScript::FastTick);

	for (RobotControl* controller : controllerList)
	{
		controller->Tick(DeltaSeconds);
	}

}