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

	if (worker->getRunValue() > 0)
	{
		MoveForward(worker->getRunValue());
		worker->setRunValue(0);
	}

	Rotate(worker->getRotateValue());
	//worker->setRotateValue(0);
}

void ARobot::MoveForward(float value)
{
	if (Controller && GEngine)
	{
		FVector Direction = GetActorForwardVector();
		AddMovementInput(Direction, value);
	}
}


void ARobot::Rotate(float value)
{
	//rotation = rotation + value;
	rotation = value;
	if (rotation > 180)rotation = rotation - 360;
	if (rotation < -180)rotation = rotation + 360;

	if (Controller && GEngine)
	{
		//AddActorLocalRotation(FRotator(0, value, 0), true);
		
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



