// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RobotWorker.h"

/**
 * 
 */
class UNREALCUP_API RobotScriptWorker : public RobotWorker
{
public:
	RobotScriptWorker(RobotControl* robotController);
	~RobotScriptWorker();
};
