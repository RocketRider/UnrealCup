// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "Robot.h"




ARobot::ARobot(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP)
{

}

void ARobot::BeginPlay()
{
	Super::BeginPlay();
	staminaTime = 0;
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