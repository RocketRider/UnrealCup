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
		moveTo,
		rotate,
		kick,
		stopBall,
		getRotation,
		getPosition,
		getStamina,
		getVisiblePlayers,
		getBallPosition,
		getGoal1Position,
		getGoal2Position
	};


private:
	TArray<Command> queueCommands;
	TArray<void*> queueParams;
	ARobot* robot;
	RobotWorker* worker;
	FPlatformProcess::FSemaphore* mutex;
	void* commandResult;


public:
	RobotControl(ARobot* robot);
	~RobotControl();
	void setWorker(RobotWorker* worker);
	void Tick(float DeltaSeconds);
	void End();
	void* call(Command function, void* param1 = NULL, void* param2 = NULL, void* param3 = NULL, void* param4 = NULL, void* param5 = NULL, void* param6 = NULL);




};
