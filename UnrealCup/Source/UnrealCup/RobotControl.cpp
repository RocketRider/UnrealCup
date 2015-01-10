// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "RobotControl.h"
#include "Robot.h"
#include "RobotWorker.h"

RobotControl::RobotControl(ARobot* robot)
{
	commandResult = NULL;
	this->robot = robot;
	this->mutex = FPlatformProcess::NewInterprocessSynchObject("robot control mutex" + robot->GetUniqueID(), true);//ADD RANDOM TO NAME!!!
	this->resultMutex = FPlatformProcess::NewInterprocessSynchObject("robot control mutex command result" + robot->GetUniqueID(), true);//ADD RANDOM TO NAME!!!
}

RobotControl::~RobotControl()
{
	FPlatformProcess::DeleteInterprocessSynchObject(mutex);
	FPlatformProcess::DeleteInterprocessSynchObject(resultMutex);
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
	mutex->Lock();
	while (queueCommands.Num()>0)
	{
		Command function = queueCommands.Pop();
		switch (function)
		{
			case move:
			{
				FFloat32* sideways = (FFloat32*)(queueParams.Pop());
				FFloat32* straight = (FFloat32*)(queueParams.Pop());
				robot->Move(straight->FloatValue, sideways->FloatValue);
				delete straight;
				delete sideways;
			}
			break;
			case rotate:
			{
				FFloat32* angle = (FFloat32*)(queueParams.Pop());
				robot->Rotate(angle->FloatValue);
				delete angle;
			}
			break;
			case getStamina:
			{

				commandResult = new FFloat32(robot->getStamina());
				resultMutex->Unlock();
			}
			break;
			case getPosition:
			{
				commandResult = new FVector(robot->getPosition());
				resultMutex->Unlock();
			}
			break;
			case getRotation:
			{
				commandResult = new FRotator(robot->getRotation());
				resultMutex->Unlock();
			}
			break;
		}

	}
	mutex->Unlock();

}


void* RobotControl::call(Command function, void* param1, void* param2, void* param3, void* param4, void* param5, void* param6)
{
	mutex->Lock();
	queueCommands.Push(function);
	
	/*
	va_list params;
	va_start(params, param);
	for (int i = 0; params[i] != NULL; i++) {
		void* data = va_arg(params, void*);
		if (data != NULL)
			queueParams.Push(data);
	}
	va_end(params);
	*/

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

	//All functions with a return value have to wait for the result
	if (function == Command::getPosition || function == Command::getRotation || function == Command::getStamina)
	{
		resultMutex->Lock();
	}

	mutex->Unlock();


	//TODO: subtract waiting time from calc time!
	/*
	resultMutex->Lock();//Wait for other Thread to unlock it or if unlocked it is finished
	resultMutex->Unlock();
	void* result = commandResult;
	commandResult = NULL;
	return result;
	*/




	return NULL;
}
