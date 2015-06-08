// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "RobotControl.h"
#include "Robot.h"
#include "RobotWorker.h"

RobotControl::RobotControl(ARobot* robot)
{
	commandResult = NULL;
	this->robot = robot;
	this->worker = NULL;

	//FString name = FString("robot control mutex"); name.AppendInt(rand());
	//this->mutex = FPlatformProcess::NewInterprocessSynchObject(name, true);
	this->mutex = FCriticalSection();

}

RobotControl::~RobotControl()
{
	//FPlatformProcess::DeleteInterprocessSynchObject(mutex);
}

void RobotControl::setWorker(RobotWorker* worker)
{
	this->worker = worker;
}

void RobotControl::End()
{
	delete worker;
	worker = NULL;
}


//Called from game thread
void RobotControl::Tick(float DeltaSeconds)
{
	if (worker)
	{
		mutex.Lock();
		while (queueCommands.Num() > 0 && worker->threadIsAllowedToRun())
		{
			Command function = queueCommands.Pop();
			switch (function)
			{
			case getTeamId:
			{
				RobotDataTypes::intStruct* result = new RobotDataTypes::intStruct();
				result->number = robot->getTeamId();
				commandResult = result;
			}
			break;
			case isKickoff:
			{
				RobotDataTypes::intStruct* result = new RobotDataTypes::intStruct();
				result->number = robot->isKickoff();
				commandResult = result;
			}
			break;
			case getTP:
			{
				RobotDataTypes::intStruct* result = new RobotDataTypes::intStruct();
				result->number = robot->getTimePlayed();
				commandResult = result;
			}
			break;
			case hasBall:
			{
				RobotDataTypes::PlayerHasBall* result = new RobotDataTypes::PlayerHasBall();
				result->hasBall = robot->hasBall();
				commandResult = result;
			}
			break;
			case move:
			{
				FFloat32* sideways = (FFloat32*)(queueParams.Pop());
				FFloat32* straight = (FFloat32*)(queueParams.Pop());
				robot->Move(straight->FloatValue, sideways->FloatValue);
				delete straight;
				delete sideways;
			}
			break;
			case moveTo:
			{
				FFloat32* speed = (FFloat32*)(queueParams.Pop());
				FFloat32* targetY = (FFloat32*)(queueParams.Pop());
				FFloat32* targetX = (FFloat32*)(queueParams.Pop());
				robot->MoveTo(targetX->FloatValue, targetY->FloatValue, speed->FloatValue);
				delete targetX;
				delete targetY;
			}
			break;
			case rotate:
			{
				FFloat32* angle = (FFloat32*)(queueParams.Pop());
				robot->Rotate(angle->FloatValue);
				delete angle;
			}
			break;
			case kick:
			{
				FFloat32* force = (FFloat32*)(queueParams.Pop());
				FVector* direction = (FVector*)(queueParams.Pop());
				FVector* dir = new FVector(*direction);
				robot->Kick(*dir, force->FloatValue);
				delete direction;
				delete force;
			}
			break;
			case stopBall:
			{
				FDateTime* stopTime = (FDateTime*)(queueParams.Pop());
				FDateTime time = FDateTime(*stopTime);
				delete stopTime;
				robot->StopBall(time);
			}
			break;
			case getStamina:
			{
				commandResult = new FFloat32(robot->getStamina());
			}
			break;
			case getPosition:
			{
				commandResult = new FVector(robot->getPosition());
			}
			break;
			case getBallPosition:
			{
				commandResult = new FVector(robot->getBallPosition());
			}
			break;
			case getGoal1Position:
			{
				commandResult = new FVector(robot->getGoal1Position());
			}
			break;
			case getGoal2Position:
			{
				commandResult = new FVector(robot->getGoal2Position());
			}
			break;
			case getRotation:
			{
				commandResult = new FRotator(robot->getRotation());
			}
			break;
			case getVisiblePlayers:
			{
				commandResult = robot->getVisiblePlayers();
			}
			break;
			case speak:
			{
				FString* text = (FString*)(queueParams.Pop());
				FString speak = FString(*text);
				delete text;
				robot->speak(speak);
			}
			break;
			case getSpoken:
			{
				commandResult = new FString(robot->getSpoken());
			}
			break;

			}

		}
		mutex.Unlock();
	}
}


void* RobotControl::call(Command function, void* param1, void* param2, void* param3, void* param4, void* param5, void* param6)
{

	if (worker == NULL)//If worker is not initialized try to wait a bit
	{
		FPlatformProcess::Sleep(0.1);
	}
	if (worker)
	{

		bool waitForResult = false;
		mutex.Lock();
		queueCommands.Push(function);
		if (param1 != NULL)
			queueParams.Push(param1);
		if (param2 != NULL)
			queueParams.Push(param2);
		if (param3 != NULL)
			queueParams.Push(param3);
		if (param4 != NULL)
			queueParams.Push(param4);
		if (param5 != NULL)
			queueParams.Push(param5);
		if (param6 != NULL)
			queueParams.Push(param6);
		mutex.Unlock();


		//All functions with a return value have to wait for the result
		if (function == Command::getPosition || function == Command::getRotation || function == Command::getStamina || function == Command::getBallPosition || 
			function == Command::getVisiblePlayers || function == Command::getGoal1Position || function == Command::getGoal2Position || function == Command::getSpoken ||
			function == Command::hasBall)
		{
			waitForResult = true;
		}


		//TODO: subtract waiting time from calc time!
		while (waitForResult)
		{
			FPlatformProcess::Sleep(0.001);
			if (commandResult != NULL || worker->threadIsAllowedToRun() == false)
				waitForResult = false;
		}

		void* result = commandResult;
		commandResult = NULL;
		return result;

	}
	return NULL;
}
