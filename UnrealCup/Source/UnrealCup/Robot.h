// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
//#include "LuaWorker.h"
#include "Robot.generated.h"


//Forward declaration
//class RobotWorker;


UCLASS()
class UNREALCUP_API ARobot : public ACharacter
{
	GENERATED_UCLASS_BODY()

	//TODO: Temporary, will be removed when scipts are selected per xml file!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lua")
	FString luaFile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	int32 team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float stamina;



	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	void Move(float straight, float sideways);
	void Rotate(float value, float DeltaSeconds);



protected:
	
	//Tick
	virtual void Tick(float DeltaSeconds) override;

	void addStamina(float DeltaSeconds);
	float staminaTime;

	//RobotWorker* worker;
	
	double rotation;

};
