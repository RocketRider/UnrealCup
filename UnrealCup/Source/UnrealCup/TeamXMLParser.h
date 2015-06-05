// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class UNREALCUP_API TeamXMLParser
{
public:
	TeamXMLParser();
	FString GetNameTeam1XML();
	FString GetNameTeam2XML();
	int32 getTimeToPlay();

	FString getPlayerName(int team, int player);
	FVector getPlayerStartLocation(int team, int player);
	FString getScriptLocation(int team, int player);
};
