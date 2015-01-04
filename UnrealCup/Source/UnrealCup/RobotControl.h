// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


class ARobot;
class RobotWorker;
/**
 * 
 */
class UNREALCUP_API RobotControl
{
public:
	RobotControl(ARobot* robot, RobotWorker* worker);
	~RobotControl();

	void Tick(float DeltaSeconds);

};
