// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "Robot.h"



ARobot::ARobot(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP)
{



}

void ARobot::BeginPlay()
{
	Super::BeginPlay();
	
	//Load lua script
	FString path = FPaths::ConvertRelativePathToFull(FPaths::GameDir()).Append(luaFile);
	LuaLoad(TCHAR_TO_ANSI(*path));
}

void ARobot::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	LuaClose();
}


void ARobot::Tick(float DeltaSeconds)
{
	MoveForward(5);
	//Rotate(10);
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
		//TODO: Does not work correctly!
		AddActorLocalRotation(FRotator(0, value, 0));
	}
}






//Lua functions:

static int32 luaMoveForward(lua_State* L) {
	double d = lua_tonumber(L, 1);  /* get argument */

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "MOVE!!!");
	}
	return 0;  /* number of results */
}

static int32 LuaUnrealLog(lua_State* L)
{
	int ArgCount = lua_gettop(L);
	FString Message;

	for (int ArgIndex = 1; ArgIndex <= ArgCount; ++ArgIndex)
	{
		if (lua_isstring(L, ArgIndex))
		{
			Message += ANSI_TO_TCHAR(lua_tostring(L, ArgIndex));
		}
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(Message));
	}
	return 0;
}






void ARobot::LuaLoad(const char* file)
{
	luaState = luaL_newstate();
	if (luaState)
	{
		luaL_openlibs(luaState);
		LuaOverridePrint();
		LuaRegisterFunctions();

		// Load the file containing the script we are going to run
		int status = luaL_loadfile(luaState, file);
		if (status) {
			fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(luaState, -1));
			LuaClose();
			//exit(1);
		}
		else
		{
			int res = lua_pcall(luaState, 0, LUA_MULTRET, 0);
		}
	}
}

void ARobot::LuaClose()
{
	if (luaState)
	{
		lua_close(luaState);
		luaState = NULL;
	}
}


void ARobot::LuaOverridePrint()
{
	static const luaL_Reg PrintOverride[] =
	{
		{ "print", LuaUnrealLog },
		{ NULL, NULL }
	};

	lua_getglobal(luaState, "_G");
	luaL_setfuncs(luaState, PrintOverride, 0);
	lua_pop(luaState, 1);
}

void ARobot::LuaRegisterFunctions()
{
	lua_pushcfunction(luaState, luaMoveForward);
	lua_setglobal(luaState, "MoveForward");
}


