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

	if (Controller)
	{


		//Works:
		//SetActorLocation(FVector(0, 0, 0));


		
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		// Limit pitch when walking or falling
		if (CharacterMovement->IsMovingOnGround() || CharacterMovement->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		Controller->GetPawn()->AddMovementInput(Direction, 1000);
		





		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("RUN!"));
		}

	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("No controller!"));
		}
	}
}
