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
	addImpulseBlueprint(direction, force);
}

void ABall::addImpulseBlueprint_Implementation(FVector direction, float force)
{

}


