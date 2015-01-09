// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "RobotControl.h"


//Forward declaration
//class ARobot;

/**
 * 
 */
class UNREALCUP_API RobotWorker : public FRunnable
{
private:
	/** Thread to run the worker FRunnable on */
	FRunnableThread* thread;
	bool allowedToRun;
	FDateTime lastTick;
	RobotControl* robotController;


protected:
	//TODO: Multiplatform 
	FPlatformProcess::FSemaphore* mutex;
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

	FVector getPosition();
	FRotator getRotation();
	float getStamina();
	void rotate(float angle);
	void move(float straight, float sideways);

	bool threadIsAllowedToRun();
	FDateTime getLastTick();
	void updateLastTick();
};
