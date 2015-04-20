// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class UNREALCUP_API TeamXMLParser
{
public:
	TeamXMLParser();
	UFUNCTION(BlueprintCallable, Category = "Xml")
	FString GetNameTeam1XML();
	UFUNCTION(BlueprintCallable, Category = "Xml")
	FString GetNameTeam2XML();

	FString getPlayerName(int team, int player);
	FVector getPlayerStartLocation(int team, int player);
	FString getScriptLocation(int team, int player);
};
