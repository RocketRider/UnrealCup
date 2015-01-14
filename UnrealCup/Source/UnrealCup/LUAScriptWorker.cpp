// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "LUAScriptWorker.h"

//Static varaibles:
TMap<lua_State*, LUAScriptWorker*> LUAScriptWorker::LuaObjectMapping;
FPlatformProcess::FSemaphore* LUAScriptWorker::globalMutex = FPlatformProcess::NewInterprocessSynchObject("Global Lua Mutex", true);






static int32 LuaMoveForward(lua_State* L)
{
	double d = lua_tonumber(L, 1);  /* get argument */

	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		worker->move(d, 0);//TODO siteways
	}

	return 0;  /* number of results */
}

static int32 LuaRotate(lua_State* L)
{
	double d = lua_tonumber(L, 1);  /* get argument */

	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		worker->rotate(d);
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
	UE_LOG(LogTemp, Warning, TEXT("Lua: %s"), *Message);

	return 0;
}

static int32 LuaGetOwnLocation(lua_State* L)
{
	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		FVector pos = worker->getPosition();

		lua_pushnumber(L, pos.X);
		lua_pushnumber(L, pos.Y);
		lua_pushnumber(L, pos.Z);
	}
	else
	{
		lua_pushnumber(L, 0);
		lua_pushnumber(L, 0);
		lua_pushnumber(L, 0);

	}
	return 3; // number of return values
}

static int32 LuaGetStamina(lua_State* L)
{
	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		lua_pushnumber(L, worker->getStamina());
	}
	else
	{
		lua_pushnumber(L, 0);
	}
	return 1; // number of return values
}

static int32 LuaAllowedToRun(lua_State* L)
{
	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		lua_pushboolean(L, worker->threadIsAllowedToRun());

		//TODO:
		/*
		FDateTime now = FDateTime::Now();
		FDateTime lastTick = worker->getLastTick();
		FTimespan dif = now - lastTick;
		double calcTime = dif.GetTotalMilliseconds();
		if (calcTime < 1) calcTime = 1;
		FPlatformProcess::Sleep(calcTime / 100);
		worker->updateLastTick();
		*/
		FPlatformProcess::Sleep(0.001);
	}
	else
	{
		//If worker is not there, sleep...
		FPlatformProcess::Sleep(0.1);
		lua_pushboolean(L, false);
	}

	return 1; // number of return values
}





















LUAScriptWorker::LUAScriptWorker(RobotControl* robotController, FString file) : RobotScriptWorker(robotController)
{
	loadLuaScript(TCHAR_TO_ANSI(*file));


	//Start the thread:
	this->thread = FRunnableThread::Create(this, TEXT("Worker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

LUAScriptWorker::~LUAScriptWorker()
{
	

	thread->Kill(true);
	delete thread;
	thread = NULL;
}



LUAScriptWorker* LUAScriptWorker::getLuaWorker(lua_State* L)
{
	globalMutex->Lock();
	LUAScriptWorker* worker = LuaObjectMapping.FindRef(L);
	globalMutex->Unlock();

	return worker;
}



//Load Lua script
void LUAScriptWorker::loadLuaScript(const char* file)
{
	luaState = luaL_newstate();
	if (luaState)
	{
		luaL_openlibs(luaState);
		overridePrint();
		registerFunctions();

		// Load the file containing the script we are going to run
		int status = luaL_loadfile(luaState, file);
		if (status) {
			UE_LOG(LogTemp, Warning, TEXT("Couldn't load file: %s\n"), lua_tostring(luaState, -1));
			freeLuaScript();
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
void LUAScriptWorker::freeLuaScript()
{
	if (luaState)
	{
		setAllowedToRun(false);
		WaitForCompletion();

		if (luaState)
		{
			lua_close(luaState);
			globalMutex->Lock();
			LuaObjectMapping.Remove(luaState);
			globalMutex->Unlock();
			luaState = NULL;
		}
	}
}


//Overwrite print function il Lua
void LUAScriptWorker::overridePrint()
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
void LUAScriptWorker::registerFunctions()
{
	//MoveForward(double Speed)
	lua_pushcfunction(luaState, LuaMoveForward);
	lua_setglobal(luaState, "MoveForward");
	lua_pushcfunction(luaState, LuaRotate);
	lua_setglobal(luaState, "Rotate");
	lua_pushcfunction(luaState, LuaGetOwnLocation);
	lua_setglobal(luaState, "GetOwnLocation");
	lua_pushcfunction(luaState, LuaGetStamina);
	lua_setglobal(luaState, "GetStamina");

	lua_pushcfunction(luaState, LuaAllowedToRun);
	lua_setglobal(luaState, "AllowedToRun");
}


//Call Tick(float DeltaSeconds) funtion in Lua script
uint32 LUAScriptWorker::Run()
{
	RobotScriptWorker::Run();

	UE_LOG(LogTemp, Warning, TEXT("LUA RUN!!!"));

	if (luaState)
	{
		/* push functions and arguments */
		lua_getglobal(luaState, "run");  /* function to be called */
		/* do the call (0 arguments, 0 result) */
		if (lua_pcall(luaState, 0, 0, 0) != 0)
			UE_LOG(LogTemp, Warning, TEXT("error running function %s"), lua_tostring(luaState, -1));
	}

	return 0;
}


//Is challed when thread should exit (thread->kill())
void LUAScriptWorker::Stop()
{
	RobotScriptWorker::Stop();

	freeLuaScript();
}



