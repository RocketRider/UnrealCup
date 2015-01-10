// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


class ARobot;
class RobotWorker;
/**
 * 
 */
class UNREALCUP_API RobotControl
{
public: enum Command
	{
		move,
		rotate,
		getRotation,
		getPosition,
		getStamina
	};


private:
	TArray<Command> queueCommands;
	TArray<void*> queueParams;
	ARobot* robot;
	FPlatformProcess::FSemaphore* mutex;


public:
	RobotControl(ARobot* robot);
	~RobotControl();
	void setWorker(RobotWorker* worker);
	void Tick(float DeltaSeconds);
	void* call(Command function, void* param, ...);




};
