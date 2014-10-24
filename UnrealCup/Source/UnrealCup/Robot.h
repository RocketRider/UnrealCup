// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Robot.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCUP_API ARobot : public ACharacter
{
	GENERATED_UCLASS_BODY()

	virtual void BeginPlay() override;
	
};
