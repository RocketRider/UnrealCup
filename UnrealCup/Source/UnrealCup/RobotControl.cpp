// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "RobotControl.h"
#include "Robot.h"
#include "RobotWorker.h"

RobotControl::RobotControl(ARobot* robot)
{
	this->robot = robot;
	this->mutex = FPlatformProcess::NewInterprocessSynchObject("robot control mutex", true);//ADD RANDOM TO NAME!!!
}

RobotControl::~RobotControl()
{
}

//Maybe not needed?
void RobotControl::setWorker(RobotWorker* worker)
{

}


void RobotControl::Tick(float DeltaSeconds)
{
	mutex->Lock();
	while (queueCommands.Num()>0)
	{
		Command function = queueCommands.Pop();
		switch (function)
		{
		case move:
			FFloat32* sideways = (FFloat32*)(queueParams.Pop());
			FFloat32* straight = (FFloat32*)(queueParams.Pop());
			
			robot->Move(straight->FloatValue, sideways->FloatValue);
			delete straight;
			delete sideways;
			break;
		//TODO: Add all commands

		}

	}
	mutex->Unlock();

}


void* RobotControl::call(Command function, void* param, ...)
{
	mutex->Lock();
	queueCommands.Push(function);
	va_list params;

	va_start(params, param);
	for (int i = 0; params[i] != NULL; i++) {
		queueParams.Push(va_arg(params, void*));
	}
	va_end(params);
	mutex->Unlock();


	//TODO: subtract waiting time from calc time!

	//All functions with a return value have to wait for the result
	if (function == Command::getPosition || function == Command::getRotation || function == Command::getStamina)
	{
		//TODO Wait for result and return ist!

	}



	return NULL;
}
