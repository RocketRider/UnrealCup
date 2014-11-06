// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "Robot.h"
#include "LuaWorker.h"




ARobot::ARobot(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP)
{
	//worker = NULL;
	
}

void ARobot::BeginPlay()
{
	Super::BeginPlay();
	
	//Load lua script
	FString path = FPaths::ConvertRelativePathToFull(FPaths::GameDir()).Append(luaFile);
	//worker = LuaWorker(this, TCHAR_TO_ANSI(*path));
	worker = new LuaWorker(this, TCHAR_TO_ANSI(*path));

	//Components of Robot:
	
	TArray<UActorComponent*, FDefaultAllocator> complist;
	GetComponents(complist);
	for (int i = 0; i < complist.Num(); i++)
	{
		if (complist[i])
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, complist[i]->GetName() + " -> " + complist[i]->GetClass()->GetName());
		}
	}
	

}

void ARobot::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//LuaClose();
	//delete worker;
	//worker = NULL;
}


void ARobot::Tick(float DeltaSeconds)
{
	FVector ownLocation = Controller->GetPawn()->GetActorLocation();
	worker->setOwnLocation(ownLocation.X, ownLocation.Y, ownLocation.Z);

	worker->mutex->Lock();
	if (worker->runcounter>0) MoveForward(worker->runcounter);
	worker->runcounter = 0;

	if(worker->rotatecounter!=0) Rotate(worker->rotatecounter);
	worker->rotatecounter = 0;
	worker->mutex->Unlock();
	//LuaTick(DeltaSeconds);
}

void ARobot::MoveForward(float value)
{
	if (Controller && GEngine)
	{
		/*
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		// Limit pitch when walking or falling
		if (CharacterMovement->IsMovingOnGround() || CharacterMovement->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		*/

		// find out which way is forward
		FVector Direction = GetActorForwardVector();
		AddMovementInput(Direction, value);
	}
}

//TODO: does not work correctly...
void ARobot::Rotate(float value)
{
	if (Controller && GEngine)
	{
		AddActorLocalRotation(FRotator(0, value, 0), true);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, "Rooo");
	}
}



