// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "TeamXMLParser.h"
#include "Runtime/XmlParser/Public/XmlFile.h"
#include "Runtime/XmlParser/Public/XmlParser.h"


FXmlFile* TeamsXml;
FXmlFile* Team1Xml;
FXmlFile* Team2Xml;

//--------------------------
// TODO Fehlerbehandlungen!  
//--------------------------

TeamXMLParser::TeamXMLParser()
{
	FString Team1Path;
	FString Team2Path;
	FString TeamPath = "Teams.xml";
	FXmlNode* node;

	TeamsXml = new FXmlFile(TeamPath);

	node = (FXmlNode*)TeamsXml->GetRootNode();

	Team1Path = node->FindChildNode("Team1")->GetAttribute("XMLLocation");
	Team2Path = node->FindChildNode("Team2")->GetAttribute("XMLLocation");

	Team1Xml = new FXmlFile(Team1Path);
	Team2Xml = new FXmlFile(Team2Path);
}


FString TeamXMLParser::GetNameTeam1XML()
{
	FXmlNode* node;
	
	if (TeamsXml->IsValid())
	{
		node = TeamsXml->GetRootNode();

		return node->FindChildNode("Team1")->GetAttribute("Name");
	}
	else
	{
		//TODO Errorausgabe
		return "Team_1";
	}
	
	return "Error";
}


FString TeamXMLParser::GetNameTeam2XML()
{
	FXmlNode* node;

	if (TeamsXml->IsValid())
	{
		node = TeamsXml->GetRootNode();

		return node->FindChildNode("Team1")->GetAttribute("Name");
	}
	else
	{
		//TODO Errorausgabe
		return "Team_2";
	}

	return "Error";
}

FString TeamXMLParser::getPlayerName(int team, int player)
{
	FXmlNode* node;
	if (team == 1) node = Team1Xml->GetRootNode();
	else if (team == 2) node = Team2Xml->GetRootNode();
	else return "Name"; // TODO Fehlerbehandlung

	return node->GetChildrenNodes()[player - 1]->GetAttribute("Name");
}

FVector TeamXMLParser::getPlayerStartLocation(int team, int player)
{
	FXmlNode* node;
	if (team == 1) node = Team1Xml->GetRootNode();
	else if (team == 2) node = Team2Xml->GetRootNode();
	else return FVector(100, 100, 0); // TODO Fehlerbehandlung

	//int x = std::stoi(node->GetChildrenNodes()[player - 1]->GetAttribute("X"));
}

FString TeamXMLParser::getScriptLocation(int team, int player)
{
	return "script.lua";
}

