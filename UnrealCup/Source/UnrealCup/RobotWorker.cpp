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

	this->thread = FRunnableThread::Create(this, TEXT("Worker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}


RobotWorker::~RobotWorker()
{
	thread->Kill(true);
	delete thread;
	thread = NULL;
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
	return FVector(0, 0, 0);
	//TODO!!!
}

FRotator RobotWorker::getRotation()
{
	return FRotator(0, 0, 0);
	//TODO!!!
}
float RobotWorker::getStamina()
{
	return 0;
	//TODO!!!
}
void RobotWorker::rotate(float angle)
{
	//TODO!!!
}
void RobotWorker::move(float straight, float sideways)
{
	//TODO!!!
}





