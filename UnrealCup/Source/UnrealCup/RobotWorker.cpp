// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "RobotWorker.h"


RobotWorker::RobotWorker(RobotControl* robotController)
{
	this->mutex = NULL;
	this->thread = NULL;
	this->robotController = robotController;
	this->allowedToRun = false;
	
	//mutex = FGenericPlatformProcess::NewInterprocessSynchObject("mutex", true, 1);

	//WINDOWS ONLY!!!! should be FGenericPlatformProcess
	this->mutex = FPlatformProcess::NewInterprocessSynchObject("mutex", true);//ADD RANDOM TO NAME!!!

	//Needs to be called in last subclass!
	//this->thread = FRunnableThread::Create(this, TEXT("Worker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}


RobotWorker::~RobotWorker()
{
	FPlatformProcess::DeleteInterprocessSynchObject(mutex);
}

bool RobotWorker::threadIsAllowedToRun()
{
	return this->allowedToRun;
}
void RobotWorker::setAllowedToRun(bool allowed)
{
	this->allowedToRun = allowed;
}

void RobotWorker::WaitForCompletion()
{
	if (thread) thread->WaitForCompletion();
}


bool RobotWorker::Init()
{
	this->allowedToRun = true;
	return true;
}



uint32 RobotWorker::Run()
{
	//Initial wait before starting
	//FPlatformProcess::Sleep(0.03);
	updateLastTick();
	return 0;
}


//Is challed when thread should exit (thread->kill())
void RobotWorker::Stop()
{
	this->allowedToRun = false;
}



FDateTime RobotWorker::getLastTick()
{
	return lastTick;
}

void RobotWorker::updateLastTick()
{
	lastTick = FDateTime::Now();
}



FVector RobotWorker::getPosition()
{
	/*
	void* result = robotController->call(RobotControl::Command::getPosition);
	FVector position = FVector(*(FVector*)result);
	delete result;
	return position;
	*/
	return FVector(0, 0, 0);
}

FRotator RobotWorker::getRotation()
{
	/*
	void* result = robotController->call(RobotControl::Command::getRotation);
	FRotator rotation = FRotator(*(FRotator*)result);
	delete result;
	return rotation;
	*/
	return FRotator(0, 0, 0);
}
float RobotWorker::getStamina()
{
	/*
	void* result = robotController->call(RobotControl::Command::getStamina);
	float stamina = ((FFloat32*)result)->FloatValue;
	delete result;
	return stamina;
	*/
	return 100;
}
void RobotWorker::rotate(float angle)
{
	robotController->call(RobotControl::Command::rotate, new FFloat32(angle));

}
void RobotWorker::move(float straight, float sideways)
{
	
	robotController->call(RobotControl::Command::move, new FFloat32(straight), new FFloat32(sideways));
}





