// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "TeamXMLParser.h"
#include "Runtime/XmlParser/Public/XmlFile.h"
#include "Runtime/XmlParser/Public/XmlParser.h"


FXmlFile* TeamsXml;
FXmlFile* Team1Xml;
FXmlFile* Team2Xml;


//TODO Fehlerbehandlungen!

TeamXMLParser::TeamXMLParser()
{
	FString Team1Path;
	FString Team2Path;
	FXmlNode* node;

	TeamsXml = new FXmlFile("Teams.xml");

	node = (FXmlNode*)TeamsXml->GetRootNode()->GetFirstChildNode();

	Team1Path = node->GetAttribute("XMLLocation");
	node =     (FXmlNode*)node->GetNextNode();
	Team2Path = node->GetAttribute("XMLLocation");

	Team1Xml = new FXmlFile(Team1Path);
	Team2Xml = new FXmlFile(Team2Path);
}


FString GetNameTeam1XML()
{
	FXmlNode* node;
	
	if (TeamsXml->IsValid())
	{
		node = TeamsXml->GetRootNode();
		node = (FXmlNode*)node->GetFirstChildNode();

		return node->GetAttribute("Name");
	}
	else
	{
		//TODO Errorausgabe
	}
	
	return "Error";
}


FString GetNameTeam2XML()
{
	FXmlNode* node;

	if (TeamsXml->IsValid())
	{
		node = TeamsXml->GetRootNode();
		node = (FXmlNode*)node->GetFirstChildNode();
		node = (FXmlNode*)node->GetNextNode();

		return node->GetAttribute("Name");
	}
	else
	{
		//TODO Errorausgabe
	}

	return "Error";
}

