// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "Robot.h"
#include "LuaWorker.h"




ARobot::ARobot(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP)
{
	worker = NULL;
}

void ARobot::BeginPlay()
{
	Super::BeginPlay();
	
	FRotator Rotation = Controller->GetControlRotation();
	rotation = Rotation.Yaw;

	staminaTime = 0;

	//Load lua script
	FString path = FPaths::ConvertRelativePathToFull(FPaths::GameDir()).Append(luaFile);
	worker = new LuaWorker(this, TCHAR_TO_ANSI(*path));

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
	delete worker;
	worker = NULL;
}


void ARobot::Tick(float DeltaSeconds)
{
	FVector ownLocation = Controller->GetPawn()->GetActorLocation();
	worker->setOwnLocation(ownLocation.X, ownLocation.Y, ownLocation.Z);
	
	addStamina(DeltaSeconds);
	worker->setStaminaValue(stamina);
	

	if (worker->getRunValue() > 0)
	{
		MoveForward(worker->getRunValue());
		worker->setRunValue(0);
	}

	Rotate(worker->getRotateValue());
	
}

void ARobot::MoveForward(float value)
{
	if (Controller && GEngine)
	{
		if ((stamina - 0.5 * value) > 0)
		{
			FVector Direction = GetActorForwardVector();
			AddMovementInput(Direction, value);
			stamina -= 0.5 * value;
		}
	}
}


void ARobot::Rotate(float value)
{
	rotation = FGenericPlatformMath::Fmod(value, 360);
	if (rotation > 180)rotation = rotation - 360;
	if (rotation < -180)rotation = rotation + 360;

	if (Controller && GEngine)
	{
		FRotator Rotation = GetActorRotation();
		double deltaRotation = rotation - Rotation.Yaw;
		if (abs(deltaRotation)>0.0001)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Rotate %f -> %f, %f"), Rotation.Yaw, rotation, rotation - Rotation.Yaw);
			
			//Should be defined by time and not at the rate of calling this function...
			if (deltaRotation > 10) deltaRotation = 10;
			if (deltaRotation < -10) deltaRotation = -10;
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



