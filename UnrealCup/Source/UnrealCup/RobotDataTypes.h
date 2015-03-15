// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class UNREALCUP_API RobotDataTypes
{
public:
	struct PlayerLocation{ int teamId; int playerId; FVector* position; };
	struct PlayerHasBall{ bool hasBall; };

	RobotDataTypes();
	~RobotDataTypes();
};
