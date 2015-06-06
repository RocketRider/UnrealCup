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
class ABall;


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FDateTime stopCommandTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	bool ballInRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	bool tryStopBall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FVector startLocation;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Move")
	void moveToLoc(FVector location, float speed);

	UFUNCTION(BlueprintNativeEvent, Category = "Ball")
	void stopBallNow();

	void Move(float straight, float sideways);
	void MoveTo(float targetX, float targetY, float speed);
	void Rotate(float value);
	void Kick(FVector direction, float force);
	void StopBall(FDateTime timer);
	
	bool hasBall();
	float getStamina();
	FRotator getRotation();
	FVector getPosition();
	FVector getBallPosition();
	FVector getGoal1Position();
	FVector getGoal2Position();
	TArray<RobotDataTypes::PlayerLocation>* getVisiblePlayers();
	int32 getTimePlayed();
	int32 getTeamId();
	int32 getPlayerId();
	void setPlayerId(int32 pId);
	void setBall(ABall* p_ball);
	void setGoals(AActor* p_goal1, AActor* p_goal2);

	void speak(FString text);
	void listen(FString text);
	FString getSpoken();

protected:
	
	//Tick
	virtual void Tick(float DeltaSeconds) override;
	float staminaTime;
	void addStamina(float DeltaSeconds);
	float rotation;
	void RotateTick(float DeltaSeconds);
	
	bool isObjectVisible(FVector objectPosition);

	ABall* ball;
	AActor* goal1;
	AActor* goal2;


	// player performance settings
	float staminaRatioMove;
	float staminaRatioKick;

	FVector oldMoveToTarget;
	float oldMoveToSpeed;
	float oldMoveToStamina; // Debug
	float oldMoveToDistance; //Debug

	float factorSpeed = 5;
	float factorKick = 500;


	TArray<FString> listenArray;
	int maxListenSize = 256;
};
