// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
//#include "LuaWorker.h"
#include "Robot.generated.h"


//Forward declaration
class LuaWorker;


UCLASS()
class UNREALCUP_API ARobot : public ACharacter
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lua")
	FString luaFile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	int32 team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float stamina;



	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	void MoveForward(float value);
	void Rotate(float value);



protected:
	
	//Tick
	virtual void Tick(float DeltaSeconds) override;

	LuaWorker* worker;
	
	double rotation;

};
