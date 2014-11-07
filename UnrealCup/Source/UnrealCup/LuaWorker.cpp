// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "LuaWorker.h"
TMap<lua_State*, LuaWorker*> LuaWorker::LuaObjectMapping;

LuaWorker::LuaWorker(ARobot* robot, const char* file)
{
	mutex = NULL;
	runcounter = 0;
	rotatecounter = 0;
	luaState = NULL;
	luafile = file;
	this->robot = robot;

	//mutex = FGenericPlatformProcess::NewInterprocessSynchObject("mutex", true, 1);

	//WINDOWS ONLY!!!! should be FGenericPlatformProcess
	mutex = FPlatformProcess::NewInterprocessSynchObject("mutex", true);//ADD RANDOM TO NAME!!!
	//globalMutex = FPlatformProcess::NewInterprocessSynchObject("Global Lua Mutex", false);


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




FPlatformProcess::FSemaphore* globalMutex = FPlatformProcess::NewInterprocessSynchObject("Global Lua Mutex", true);
static int32 LuaMoveForward(lua_State* L)
{
	double d = lua_tonumber(L, 1);  /* get argument */

	globalMutex->Lock();
	LuaWorker* worker = LuaWorker::LuaObjectMapping[L];
	globalMutex->Unlock();


	worker->mutex->Lock();
	worker->runcounter = worker->runcounter + d;
	worker->mutex->Unlock();

	return 0;  /* number of results */
}

static int32 LuaRotate(lua_State* L)
{
	double d = lua_tonumber(L, 1);  /* get argument */

	globalMutex->Lock();
	LuaWorker* worker = LuaWorker::LuaObjectMapping[L];
	globalMutex->Unlock();

	worker->mutex->Lock();
	worker->rotatecounter = worker->rotatecounter + d;
	worker->mutex->Unlock();

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

static int32 LuaGetOwnLocation(lua_State* L)
{
	globalMutex->Lock();
	LuaWorker* worker = LuaWorker::LuaObjectMapping[L];
	globalMutex->Unlock();

	lua_pushnumber(L, worker->getOwnX());
	lua_pushnumber(L, worker->getOwnY());
	lua_pushnumber(L, worker->getOwnZ());

	return 3; // number of return values
}

void LuaWorker::setOwnLocation(double x, double y, double z)
{
	mutex->Lock();
	ownX = x;
	ownY = y;
	ownZ = z;
	mutex->Unlock();
}

double LuaWorker::getOwnX()
{
	mutex->Lock();
	double returnValue = ownX;
	mutex->Unlock();
	return returnValue;
}

double LuaWorker::getOwnY()
{
	mutex->Lock();
	double returnValue = ownY;
	mutex->Unlock();
	return returnValue;
}

double LuaWorker::getOwnZ()
{
	mutex->Lock();
	double returnValue = ownZ;
	mutex->Unlock();
	return returnValue;
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
			globalMutex->Lock();
			LuaObjectMapping.Add(luaState, this);
			globalMutex->Unlock();
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
		globalMutex->Lock();
		LuaObjectMapping.Remove(luaState);
		globalMutex->Unlock();
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
	lua_pushcfunction(luaState, LuaGetOwnLocation);
	lua_setglobal(luaState, "GetOwnLocation");

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

