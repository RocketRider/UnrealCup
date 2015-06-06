// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "RobotWorker.h"


RobotWorker::RobotWorker(RobotControl* robotController)
{
	//this->mutex = NULL;
	this->thread = NULL;
	this->robotController = robotController;
	this->allowedToRun = false;
	
	//mutex = FGenericPlatformProcess::NewInterprocessSynchObject("mutex", true, 1);

	//WINDOWS ONLY!!!! should be FGenericPlatformProcess
	//FString name = FString("RobotWorker mutex"); name.AppendInt(rand());
	//this->mutex = FPlatformProcess::NewInterprocessSynchObject(name, true);

	this->mutex = FCriticalSection();

	//Needs to be called in last subclass!
	//this->thread = FRunnableThread::Create(this, TEXT("Worker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}


RobotWorker::~RobotWorker()
{
	//FPlatformProcess::DeleteInterprocessSynchObject(mutex);
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
	FPlatformProcess::Sleep(0.1);
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


int32 RobotWorker::getTimePlayed()
{
	void* result = robotController->call(RobotControl::Command::getTP);
	int32 timePlayed = (int32)result;
	return timePlayed;
}


FVector RobotWorker::getPosition()
{
	
	void* result = robotController->call(RobotControl::Command::getPosition);
	if (result != NULL)
	{
		FVector position = FVector(*(FVector*)result);
		delete (FVector*)result;
		return position;
	}
	return FVector(0, 0, 0);
}

FVector RobotWorker::getBallPosition()
{
	void* result = robotController->call(RobotControl::Command::getBallPosition);
	if (result != NULL)
	{
		FVector ballPosition = FVector(*(FVector*)result);
		delete (FVector*)result;
		return ballPosition;
	}
	return FVector(0, 0, 0);
}


FVector RobotWorker::getGoal1Position()
{
	void* result = robotController->call(RobotControl::Command::getGoal1Position);
	if (result != NULL)
	{
		FVector goal1Position = FVector(*(FVector*)result);
		delete (FVector*)result;
		return goal1Position;
	}
	return FVector(0, 0, 0);
}


FVector RobotWorker::getGoal2Position()
{
	void* result = robotController->call(RobotControl::Command::getGoal2Position);
	if (result != NULL)
	{
		FVector goal2Position = FVector(*(FVector*)result);
		delete (FVector*)result;
		return goal2Position;
	}
	return FVector(0, 0, 0);
}

FRotator RobotWorker::getRotation()
{
	
	void* result = robotController->call(RobotControl::Command::getRotation);
	if (result != NULL)
	{
		FRotator rotation = FRotator(*(FRotator*)result);
		delete (FRotator*)result;
		return rotation;
	}
	return FRotator(0, 0, 0);
}
bool RobotWorker::hasBall()
{
	void* result = robotController->call(RobotControl::Command::hasBall);
	if (result != NULL)
	{
		bool hasBall = ((RobotDataTypes::PlayerHasBall*)result)->hasBall;
		delete (RobotDataTypes::PlayerHasBall*)result;
		return hasBall;
	}
	return false;
}
float RobotWorker::getStamina()
{
	
	
	void* result = robotController->call(RobotControl::Command::getStamina);
	if (result != NULL)
	{
		float stamina = ((FFloat32*)result)->FloatValue;
		delete (FFloat32*)result;
		return stamina;
	}
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

void RobotWorker::moveTo(float targetX, float targetY, float speed)
{
	robotController->call(RobotControl::Command::moveTo, new FFloat32(targetX), new FFloat32(targetY), new FFloat32(speed));
}

void RobotWorker::kick(FVector direction, float force)
{
	robotController->call(RobotControl::Command::kick, new FVector(direction), new FFloat32(force));
}

void RobotWorker::stopBall()
{
	robotController->call(RobotControl::Command::stopBall, new FDateTime(FDateTime::Now()));
}

// Speicher MUSS nach dem lesen freigegeben werden!!!
TArray<RobotDataTypes::PlayerLocation>* RobotWorker::getVisiblePlayersAbsolute()
{
	void* result = robotController->call(RobotControl::Command::getVisiblePlayers);
	if (result != NULL)
	{
		TArray<RobotDataTypes::PlayerLocation>* visiblePlayersAbsolute = (TArray<RobotDataTypes::PlayerLocation>*)result;
		return visiblePlayersAbsolute;
	}
	return NULL;
}


FString RobotWorker::getSpoken()
{


	void* result = robotController->call(RobotControl::Command::getSpoken);
	if (result != NULL)
	{
		FString text = FString(*(FString*)result);
		delete (FString*)result;
		return text;
	}
	return FString("");
}
void RobotWorker::speak(FString text)
{
	robotController->call(RobotControl::Command::speak, new FString(text));

}


