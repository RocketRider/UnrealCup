// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "Robot.h"
#include "Ball.h"




ARobot::ARobot(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP)
{
}

void ARobot::BeginPlay()
{
	Super::BeginPlay();
	staminaTime = 0;
	hasBall = true;
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
		if ((stamina - 0.4 * straight) > 0)
		{
			AddMovementInput(Direction, straight);
			stamina -= 0.4 * straight;
		}
		else
		{
			stamina = 0;
			AddMovementInput(Direction, 0.1);
		}

	}
}

// Move to a specific location on the map
void ARobot::MoveTo(float targetX, float targetY)
{
	// adapt the height
	FVector targetPosition = FVector(targetX, targetY, getPosition().Z);

	// calculate distance to move & needed stamina
	float distance = FMath::Sqrt( pow(targetPosition.X - getPosition().X, 2) + pow(targetPosition.Y - getPosition().Y, 2) );
	float neededStamina = distance * 0.4;

	// enough stamina -> run full speed
	if ((stamina - neededStamina) > 0)
	{
		// TODO: Set Movement speed to max
		
		stamina -= neededStamina;
	}

	// not enough stamina -> move slower
	else
	{
		float movSpeedRatio = neededStamina / stamina;

		// TODO: Set Movement speed
		
		stamina = 0;
	}
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
	if (hasBall && ball != nullptr)
	{
		float neededStamina = force * 0;

		// enough stamina -> kick full force
		if ((stamina - neededStamina) > 0)
		{
			ball->addForce(direction, force);
			stamina -= neededStamina;
		}

		// not enough stamina -> kick weaker
		else
		{
			float forceRatio = neededStamina / stamina;
			ball->addForce(direction, force * forceRatio);
			stamina = 0;
		}
	}
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
		
		float angle = FMath::RadiansToDegrees(atan2(ownLocation.Y - robot->getPosition().Y, ownLocation.X - robot->getPosition().X));
		float deltaAngle = angle - ownRotation.Yaw;
		if (deltaAngle < -180) deltaAngle += 360;
		if (deltaAngle > 180) deltaAngle -= 360;
		if (abs(deltaAngle <= HalfFieldOfView)) visiblePlayerLocations->Add(RobotDataTypes::PlayerLocation{ robot->getTeamId(), robot->getPlayerId(), new FVector(robot->getPosition()) });
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