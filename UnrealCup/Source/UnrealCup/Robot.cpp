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
	Rotate(10);
}



void ARobot::MoveForward(float value)
{

	if (Controller && GEngine)
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
		AddMovementInput(Direction, value);

	}

}


void ARobot::Rotate(float value)
{
	if (Controller && GEngine)
	{
		//Does not work correctly!
		AddActorLocalRotation(FRotator(0, value, 0));
	}

}