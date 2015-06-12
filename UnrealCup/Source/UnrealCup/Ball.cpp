// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "Ball.h"

FVector ABall::getLocation()
{
	// ToDo find better function for Location
	return GetPawnViewLocation();
}

void ABall::addForce(FVector direction, float force)
{
	if (force < 1)
		force = 1;
	if (force > 100000)//Just to makte sure that it is not +INF
		force = 100000;

	//NEVER CALL THIS FUNCTION WITH INVALID NUMBERS LIKE 1.INF. It will crash the collision system!!!
	direction.Normalize();
	direction.X = direction.X + ((float)rand() / RAND_MAX / 10.0);
	direction.X = direction.X - ((float)rand() / RAND_MAX / 10.0);

	direction.Y = direction.Y + ((float)rand() / RAND_MAX / 10.0);
	direction.Y = direction.Y - ((float)rand() / RAND_MAX / 10.0);

	direction.Z = direction.Z + ((float)rand() / RAND_MAX / 10.0);
	direction.Z = direction.Z - ((float)rand() / RAND_MAX / 10.0);

	addImpulseBlueprint(direction, force);
	

	//UE_LOG(LogTemp, Warning, TEXT("kick: %f,%f,%f   %f"), direction.X, direction.Y, direction.Z, force);
	

}

void ABall::addImpulseBlueprint_Implementation(FVector direction, float force)
{

}

void ABall::setKickoffState_Implementation(bool state)
{

}


