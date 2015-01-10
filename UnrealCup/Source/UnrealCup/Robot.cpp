// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "Robot.h"
//#include "RobotWorker.h"




ARobot::ARobot(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP)
{
	//worker = NULL;
}

void ARobot::BeginPlay()
{
	Super::BeginPlay();
	
	FRotator Rotation = Controller->GetControlRotation();
	rotation = Rotation.Yaw;

	staminaTime = 0;

	//Load lua script
	//FString path = FPaths::ConvertRelativePathToFull(FPaths::GameDir()).Append(luaFile);
	//worker = new RobotWorker(this, TCHAR_TO_ANSI(*path));

	//Components of Robot:
	/*
	TArray<UActorComponent*, FDefaultAllocator> complist;
	GetComponents(complist);
	for (int i = 0; i < complist.Num(); i++)
	{
		if (complist[i])
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, complist[i]->GetName() + " -> " + complist[i]->GetClass()->GetName());
		}
	}
	*/

}

void ARobot::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//delete worker;
	//worker = NULL;
}


void ARobot::Tick(float DeltaSeconds)
{
	addStamina(DeltaSeconds);

	//FVector ownLocation = Controller->GetPawn()->GetActorLocation();
	//worker->setOwnLocation(ownLocation.X, ownLocation.Y, ownLocation.Z);
	//worker->setStaminaValue(stamina);
	
	/*if (worker->getRunValue() > 0)
	{
		MoveForward(worker->getRunValue(), DeltaSeconds);
		worker->setRunValue(0);
	}

	Rotate(worker->getRotateValue(), DeltaSeconds);
	*/
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


void ARobot::Rotate(float value, float DeltaSeconds)
{
	rotation = FGenericPlatformMath::Fmod(value, 360);
	if (rotation > 180)rotation = rotation - 360;
	if (rotation < -180)rotation = rotation + 360;

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



