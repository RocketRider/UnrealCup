// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "AIController.h"
#include "AIModule.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
//#include "LuaWorker.h"
#include "RobotDataTypes.h"
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
	int32 playerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	int32 HalfFieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float stamina;



	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void Move(float straight, float sideways);
	void MoveTo(float targetX, float targetY);
	void Rotate(float value);
	
	float getStamina();
	FRotator getRotation();
	FVector getPosition();
	TArray<RobotDataTypes::PlayerLocation>* getVisiblePlayers();
	int32 getTeamId();
	int32 getPlayerId();
	void setPlayerId(int32 pId);


protected:
	
	//Tick
	virtual void Tick(float DeltaSeconds) override;
	float staminaTime;
	void addStamina(float DeltaSeconds);
	float rotation;
	void RotateTick(float DeltaSeconds);
	




};
