// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "Robot.h"
TMap<lua_State*, ARobot*> ARobot::LuaObjectMapping;




ARobot::ARobot(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP)
{
	luaState = NULL;
}

void ARobot::BeginPlay()
{
	Super::BeginPlay();
	
	//Load lua script
	FString path = FPaths::ConvertRelativePathToFull(FPaths::GameDir()).Append(luaFile);
	LuaLoad(TCHAR_TO_ANSI(*path));

	

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
	LuaClose();
}


void ARobot::Tick(float DeltaSeconds)
{
	LuaTick(DeltaSeconds);
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
void ARobot::Rotate(float value)
{
	if (Controller && GEngine)
	{
		AddActorLocalRotation(FRotator(0, value, 0));
	}
}






//Lua functions:

static int32 LuaMoveForward(lua_State* L) {
	double d = lua_tonumber(L, 1);  /* get argument */

	
	ARobot *robot = ARobot::LuaObjectMapping[L];
	if (robot)
	{
		robot->MoveForward(d);
	}
	
	return 0;  /* number of results */
}

static int32 LuaRotate(lua_State* L) {
	double d = lua_tonumber(L, 1);  /* get argument */


	ARobot *robot = ARobot::LuaObjectMapping[L];
	if (robot)
	{
		robot->Rotate(d);
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







//Load Lua script
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
			LuaObjectMapping.Add(luaState, this);
			int res = lua_pcall(luaState, 0, LUA_MULTRET, 0);
		}
	}
}


//Free Lua script
void ARobot::LuaClose()
{
	if (luaState)
	{
		lua_close(luaState);
		LuaObjectMapping.Remove(luaState);
		luaState = NULL;
	}
}


//Overwrite print function il Lua
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


//Register Functions
void ARobot::LuaRegisterFunctions()
{
	//MoveForward(double Speed)
	lua_pushcfunction(luaState, LuaMoveForward);
	lua_setglobal(luaState, "MoveForward");
	lua_pushcfunction(luaState, LuaRotate);
	lua_setglobal(luaState, "Rotate");

}


//Call Tick(float DeltaSeconds) funtion in Lua script
void ARobot::LuaTick(float DeltaSeconds)
{
	if (luaState)
	{
		/* push functions and arguments */
		lua_getglobal(luaState, "tick");  /* function to be called */
		lua_pushnumber(luaState, DeltaSeconds);   /* push 1st argument */
		/* do the call (1 arguments, 0 result) */
		if (lua_pcall(luaState, 1, 0, 0) != 0)
			printf("error running function `f': %s", lua_tostring(luaState, -1));
	}
}

