// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "Robot.h"


ARobot::ARobot(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{



}

void ARobot::BeginPlay()
{
	Super::BeginPlay();

}

void ARobot::Tick(float DeltaSeconds)
{

	MoveForward(5);

}



void ARobot::MoveForward(float value)
{

	if (Controller)
	{
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		// Limit pitch when walking or falling
		if (CharacterMovement->IsMovingOnGround() || CharacterMovement->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		Controller->GetPawn()->AddMovementInput(Direction, value);

	}

}