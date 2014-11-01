// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "LuaWorker.h"
TMap<lua_State*, LuaWorker*> LuaWorker::LuaObjectMapping;

LuaWorker::LuaWorker(ARobot* robot, const char* file) 
{
	luaState = NULL;
	luafile = file;
	this->robot = robot;

	thread = FRunnableThread::Create(this, TEXT("LuaWorker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

LuaWorker* LuaWorker::LuaInit(ARobot* robot, const char* file)
{
	return new LuaWorker(robot, file);
}


LuaWorker::~LuaWorker()
{
	thread->Kill();
	delete thread;
	thread = NULL;
}



bool LuaWorker::Init()
{
	LuaLoad(luafile);
	return true;
}


uint32 LuaWorker::Run()
{
	//Initial wait before starting
	//FPlatformProcess::Sleep(0.03);

	LuaRun();

	return 0;
}


void LuaWorker::Stop()
{
	//LuaClose();
}







struct FParameters
{
	lua_State* L;
	double d;
};

static void LuaMoveForward_gameThread(FParameters* params)
{

	ARobot* robot = LuaWorker::LuaObjectMapping[params->L]->robot;
	if (robot)
	{
		robot->MoveForward(params->d);
	}

	//Free params after usage!
	delete params;
}

static int32 LuaMoveForward(lua_State* L)
{
	double d = lua_tonumber(L, 1);  /* get argument */

	FParameters* params = new FParameters();
	params->d = d;
	params->L = L;

	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady
		(
		FSimpleDelegateGraphTask::FDelegate::CreateStatic(&LuaMoveForward_gameThread, params)
		, TStatId()
		, nullptr
		, ENamedThreads::GameThread
		);


	//Do not free params here, it is needed in the asyncron task!
	return 0;  /* number of results */
}

static int32 LuaRotate(lua_State* L)
{
	double d = lua_tonumber(L, 1);  /* get argument */

	/*
	ARobot *robot = ARobot::LuaObjectMapping[L];
	if (robot)
	{
		robot->Rotate(d);
	}
	*/

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
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(Message));
	}
	return 0;
}







//Load Lua script
void LuaWorker::LuaLoad(const char* file)
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
void LuaWorker::LuaClose()
{
	if (luaState)
	{
		lua_close(luaState);
		//LuaObjectMapping.Remove(luaState);
		luaState = NULL;
	}
}


//Overwrite print function il Lua
void LuaWorker::LuaOverridePrint()
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
void LuaWorker::LuaRegisterFunctions()
{
	//MoveForward(double Speed)
	lua_pushcfunction(luaState, LuaMoveForward);
	lua_setglobal(luaState, "MoveForward");
	lua_pushcfunction(luaState, LuaRotate);
	lua_setglobal(luaState, "Rotate");

}


//Call Tick(float DeltaSeconds) funtion in Lua script
void LuaWorker::LuaRun()
{
	if (luaState)
	{
		/* push functions and arguments */
		lua_getglobal(luaState, "run");  /* function to be called */
		/* do the call (0 arguments, 0 result) */
		if (lua_pcall(luaState, 0, 0, 0) != 0)
			printf("error running function `f': %s", lua_tostring(luaState, -1));
	}
}

