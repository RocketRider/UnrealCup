// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "Robot.h"
#include "Ball.h"




ARobot::ARobot(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP)
{
	// tweak player performance
	staminaRatioMove = 1 / (5000 * factorSpeed); // 0.00004; // with this value the player can move half the field with full speed
	staminaRatioKick = 1 / factorKick / 10; // 0.001; // with this value the player can kick ten times with full force
}

void ARobot::BeginPlay()
{
	Super::BeginPlay();
	staminaTime = 0;
	//ballInRange = true;//false;
	tryStopBall = false;
	oldMoveToTarget = FVector::ZeroVector;
	oldMoveToSpeed = 0;
	oldMoveToStamina = 0;
	oldMoveToDistance = 0;
}

void ARobot::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

}


void ARobot::Tick(float DeltaSeconds)
{
	addStamina(DeltaSeconds);
	RotateTick(DeltaSeconds);
}

void ARobot::Move(float straight, float sideways)
{
	//TODO: add sideways walking
	if (Controller && GEngine)
	{
		FVector Direction = GetActorForwardVector();
		if ((stamina - staminaRatioMove * straight) > 0)
		{
			AddMovementInput(Direction, straight);
			stamina -= staminaRatioMove * straight;
		}
		else
		{
			stamina = 0;
			AddMovementInput(Direction, 0.1);
		}
	}
}

// Move to a specific location on the map
void ARobot::MoveTo(float targetX, float targetY, float speed)
{
	// return stamina from last MoveTo
	if (!oldMoveToTarget.IsZero())
	{
		FVector pos = getPosition();
		float distance = FMath::Sqrt(pow(oldMoveToTarget.X - pos.X, 2) + pow(oldMoveToTarget.Y - pos.Y, 2));
		float returnStamina = distance * oldMoveToSpeed *staminaRatioMove;
		if (returnStamina > this->oldMoveToStamina)
			UE_LOG(LogTemp, Error, TEXT("stamina error"));
			// TODO: Distanz zum alten Target größer als vor dem MoveTo?!? Sollte eigentlich nicht gehen, Spieler läuft ja in Richtung von seinem Ziel
		stamina += returnStamina;
		if (stamina > 100)
			stamina = 100;
	}

	// clamp input speed
	if (speed > 100)
		speed = 100;
	float moveSpeed = speed * factorSpeed;
	
	// adapt the height
	FVector targetPosition = FVector(targetX, targetY, getPosition().Z);

	// calculate distance to move & needed stamina
	float distance = FMath::Sqrt( pow(targetPosition.X - getPosition().X, 2) + pow(targetPosition.Y - getPosition().Y, 2) );
	float neededStamina = distance * moveSpeed * staminaRatioMove;

	FVector ownLocation = GetActorLocation();
	float angle = FMath::RadiansToDegrees(atan2(targetY - ownLocation.Y, targetX - ownLocation.X));
	Rotate(angle);

	if (neededStamina > stamina) // not enough stamina -> run slower
	{
		moveSpeed = stamina / (distance * staminaRatioMove);
		neededStamina = distance * moveSpeed * staminaRatioMove;
	}

	//Random speed(+- 10%):
	moveSpeed = moveSpeed*(((float)rand() / (float)RAND_MAX / 10.0) + 0.90);

	stamina -= neededStamina;
	moveToLoc(targetPosition, moveSpeed);

	oldMoveToSpeed = moveSpeed;
	oldMoveToTarget = targetPosition;
	oldMoveToStamina = neededStamina;
	oldMoveToDistance = distance;
}

// For usage of event in blueprint
void ARobot::moveToLoc_Implementation(FVector location, float speed)
{

}

void ARobot::Rotate(float angle)
{

	rotation = FGenericPlatformMath::Fmod(angle, 360);
	if (rotation > 180)rotation = rotation - 360;
	if (rotation < -180)rotation = rotation + 360;
}

void ARobot::RotateTick(float DeltaSeconds)
{
	if (Controller && GEngine)
	{
		FRotator Rotation = GetActorRotation();
		double deltaRotation = rotation - Rotation.Yaw;
		if (abs(deltaRotation)>180) deltaRotation = Rotation.Yaw - rotation;

		if (abs(deltaRotation)>0.0001)
		{
			float maxRotate = DeltaSeconds*400;

			if (deltaRotation > maxRotate) deltaRotation = maxRotate;
			if (deltaRotation < -maxRotate) deltaRotation = -maxRotate;
			AddActorLocalRotation(FRotator(0, deltaRotation, 0));
		}

	}
}

void ARobot::Kick(FVector direction, float force)
{
	//return;

	if (ballInRange && ball != nullptr && stamina > 0)
	{
		if (force > 100)
			force = 100;
		force *= factorKick;

		float neededStamina = force * staminaRatioKick;

		// enough stamina -> kick full force
		if (stamina >= neededStamina)
		{
			ball->addForce(direction, force);
			stamina -= neededStamina;
		}
		else// not enough stamina -> kick weaker
		{
			float force = stamina / staminaRatioKick;
			ball->addForce(direction, force);
			stamina = 0;
		}
	}
}

// Stop the Ball
void ARobot::StopBall(FDateTime timer)
{
	//return;

	// ball is already in range: try to stop it right away
	// and dont try to stop it again when it enters the range again
	if (ballInRange)
	{
		tryStopBall = false;
		stopBallNow();
	}
	// set a timer and try to stop the ball when it enters the range
	// the allowed time between stop-Command and actually stopping can
	// be set in the blueprint
	else
	{
		tryStopBall = true;
		stopCommandTime = timer;
	}	
}

// stop ball now (event for blueprint)
void ARobot::stopBallNow_Implementation()
{

}

void ARobot::addStamina(float DeltaSeconds)
{
	staminaTime += DeltaSeconds;
	if (staminaTime >= 0.01)
	{
		staminaTime -= 0.01;
		stamina += 1;
		if (stamina > 100) stamina = 100;
	}
}

float ARobot::getStamina()
{
	return stamina;
}

FRotator ARobot::getRotation()
{
	return  GetActorRotation();
}

FVector ARobot::getPosition()
{
	return GetActorLocation();
}

bool ARobot::isObjectVisible(FVector objectPosition)
{
	FVector ownLocation = GetActorLocation();
	FRotator ownRotation = GetActorRotation();
	float distance = sqrt(pow(ownLocation.X - objectPosition.X, 2) + pow(ownLocation.Y - objectPosition.X, 2));

	if (distance < 500) //See everyting in a distance of 5 meter
	{
		return true;
	}

	//See everypting in the field of view
	float angle = FMath::RadiansToDegrees(atan2(ownLocation.Y - objectPosition.Y, ownLocation.X - objectPosition.X));
	float deltaAngle = angle - ownRotation.Yaw + 180;
	if (deltaAngle < -180) deltaAngle += 360;
	{
		if (deltaAngle > 180) deltaAngle -= 360;
		{
			if (abs(deltaAngle) <= HalfFieldOfView)
			{
				return true;
			}
		}
	}

	return false;
}


FVector ARobot::getBallPosition()
{
	FVector returnvalue = FVector(0, 0, 0);
	if (ball != nullptr)
	{
		if (isObjectVisible(ball->getLocation()))
		{
			returnvalue = ball->getLocation();
		}
	}
	return returnvalue;
}

FVector ARobot::getGoal1Position()
{
	if (goal1 != NULL)
		return goal1->GetActorLocation();
	else
		return FVector(0, 0, 0);
}
FVector ARobot::getGoal2Position()
{
	if (goal2 != NULL)
		return goal2->GetActorLocation();
	else
		return FVector(0, 0, 0);
}

TArray<RobotDataTypes::PlayerLocation>* ARobot::getVisiblePlayers()
{
	FVector ownLocation = GetActorLocation();
	FRotator ownRotation = GetActorRotation();
	TArray<RobotDataTypes::PlayerLocation>* visiblePlayerLocations = new TArray<RobotDataTypes::PlayerLocation>();

	//Iterate through all Robots	
	for (TActorIterator<ARobot> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ARobot* robot = Cast<ARobot>(*ActorItr);
		if (this == robot) continue;
		
		if (isObjectVisible(robot->getPosition()))
		{
			visiblePlayerLocations->Add(RobotDataTypes::PlayerLocation{ robot->getTeamId(), robot->getPlayerId(), new FVector(robot->getPosition()) });
		}
	}

	return visiblePlayerLocations;
}

int32 ARobot::getTeamId()
{
	return team;
}


int32 ARobot::getPlayerId()
{
	return playerId;
}

void ARobot::setPlayerId(int32 pId)
{
	playerId = pId;
}

void ARobot::setBall(ABall*	p_ball)
{
	ball = p_ball;
}

void ARobot::setGoals(AActor* p_goal1, AActor* p_goal2)
{
	goal1 = p_goal1;
	goal2 = p_goal2;
}



void ARobot::speak(FString text)
{

	//Speak to all Robots:
	for (TActorIterator<ARobot> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ARobot* robot = Cast<ARobot>(*ActorItr);
		if (this == robot) continue;

		robot->listen(text);
	}
}
void ARobot::listen(FString text)
{
	//If full, remove last item... Not sure if it is really the last added one...
	if (listenArray.Num() >= maxListenSize)
	{
		//TODO: Test which one is the correct one:
		//listenArray.RemoveAt(listenArray.Num - 1, 1);
		listenArray.RemoveAt(0, 1);
	}

	listenArray.Push(text);
}
FString ARobot::getSpoken()
{
	if (listenArray.Num() > 0)
	{
		return listenArray.Pop();
	}

	return FString("");
}