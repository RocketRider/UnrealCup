// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "RobotControl.h"
#include "RobotDataTypes.h"


//Forward declaration
//class ARobot;

/**
 * 
 */
class UNREALCUP_API RobotWorker : public FRunnable
{
private:
	/** Thread to run the worker FRunnable on */
	
	bool allowedToRun;
	FDateTime lastTick;
	RobotControl* robotController;


protected:
	FRunnableThread* thread;

	//TODO: Multiplatform 
	//FPlatformProcess::FSemaphore* mutex;
	FCriticalSection mutex;

	void setAllowedToRun(bool allowed);
	void WaitForCompletion();


	// Begin FRunnable interface.
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	// End FRunnable interface

public:
	RobotWorker(RobotControl* robotController);
	~RobotWorker();

	bool hasBall();
	FVector getPosition();
	FVector getBallPosition();
	FVector getGoal1Position();
	FVector getGoal2Position();
	FRotator getRotation();
	float getStamina();
	int32 getTeamId();
	int32 isKickoff();
	int32 getTimePlayed();
	void rotate(float angle);
	void move(float straight, float sideways);
	void moveTo(float targetX, float targetY, float speed);
	void kick(FVector direction, float force);
	void stopBall();
	TArray<RobotDataTypes::PlayerLocation>* getVisiblePlayersAbsolute();

	void speak(FString text);
	FString getSpoken();

	bool threadIsAllowedToRun();
	FDateTime getLastTick();
	void updateLastTick();
};
