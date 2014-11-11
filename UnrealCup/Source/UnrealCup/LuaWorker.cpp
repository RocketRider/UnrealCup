// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "LuaWorker.h"
TMap<lua_State*, LuaWorker*> LuaWorker::LuaObjectMapping;
FPlatformProcess::FSemaphore* LuaWorker::globalMutex = FPlatformProcess::NewInterprocessSynchObject("Global Lua Mutex", true);

LuaWorker::LuaWorker(ARobot* robot, const char* file)
{
	mutex = NULL;
	runValue = 0;
	rotateValue = 0;
	luaState = NULL;
	luafile = file;
	this->robot = robot;
	allowedToRun = false;
	stamina = 0;
	thread = NULL;

	//mutex = FGenericPlatformProcess::NewInterprocessSynchObject("mutex", true, 1);

	//WINDOWS ONLY!!!! should be FGenericPlatformProcess
	mutex = FPlatformProcess::NewInterprocessSynchObject("mutex", true);//ADD RANDOM TO NAME!!!


	thread = FRunnableThread::Create(this, TEXT("LuaWorker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

LuaWorker* LuaWorker::LuaInit(ARobot* robot, const char* file)
{
	return new LuaWorker(robot, file);
}


LuaWorker::~LuaWorker()
{
	thread->Kill(true);
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
	updateLastTick();
	LuaRun();

	return 0;
}

//Is challed when thread should exit (thread->kill())
void LuaWorker::Stop()
{
	LuaClose();
}



LuaWorker* LuaWorker::getLuaWorker(lua_State* L)
{
	globalMutex->Lock();
	LuaWorker* worker = LuaObjectMapping.FindRef(L);
	globalMutex->Unlock();

	return worker;
}


static int32 LuaMoveForward(lua_State* L)
{
	double d = lua_tonumber(L, 1);  /* get argument */

	LuaWorker* worker = LuaWorker::getLuaWorker(L);
	if (worker)
	{
		if (worker->getStaminaValue() >= 0)
		{
			worker->setRunValue(worker->getRunValue() + d);
		}
	}

	return 0;  /* number of results */
}

static int32 LuaRotate(lua_State* L)
{
	double d = lua_tonumber(L, 1);  /* get argument */

	LuaWorker* worker = LuaWorker::getLuaWorker(L);
	if (worker)
	{
		//worker->setRotateValue(worker->getRotateValue() + d);
		worker->setRotateValue(d);
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


	//NOT THREADSAFE!!!
	/*
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(Message));
	}
	*/
	return 0;
}

static int32 LuaGetOwnLocation(lua_State* L)
{
	LuaWorker* worker = LuaWorker::getLuaWorker(L);
	if (worker)
	{
		lua_pushnumber(L, worker->getOwnX());
		lua_pushnumber(L, worker->getOwnY());
		lua_pushnumber(L, worker->getOwnZ());
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
	LuaWorker* worker = LuaWorker::getLuaWorker(L);
	if (worker)
	{
		lua_pushnumber(L, worker->getStaminaValue());
	}
	else
	{
		lua_pushnumber(L, 0);
	}
	return 1; // number of return values
}

static int32 LuaAllowedToRun(lua_State* L)
{
	LuaWorker* worker = LuaWorker::getLuaWorker(L);
	if (worker)
	{
		lua_pushboolean(L, worker->isAllowedToRun());

		FDateTime now = FDateTime::Now();
		FDateTime lastTick = worker->getLastTick();
		FTimespan dif = now - lastTick;
		double calcTime = dif.GetTotalMilliseconds();
		if (calcTime < 1) calcTime = 1;
		FPlatformProcess::Sleep(calcTime/100);

		worker->updateLastTick();
	}
	else
	{
		//If worker is not there, sleep...
		FPlatformProcess::Sleep(0.1);
		lua_pushboolean(L, false);
	}

	return 1; // number of return values
}


void LuaWorker::setOwnLocation(double x, double y, double z)
{
	if (mutex)
	{
		mutex->Lock();
		ownX = x;
		ownY = y;
		ownZ = z;
		mutex->Unlock();
	}
}

double LuaWorker::getOwnX()
{
	if (mutex)
	{
		mutex->Lock();
		double returnValue = ownX;
		mutex->Unlock();
		return returnValue;
	}
	return 0;
}

double LuaWorker::getOwnY()
{
	if (mutex)
	{
		mutex->Lock();
		double returnValue = ownY;
		mutex->Unlock();
		return returnValue;
	}
	return 0;
}

double LuaWorker::getOwnZ()
{
	if (mutex)
	{
		mutex->Lock();
		double returnValue = ownZ;
		mutex->Unlock();
		return returnValue;
	}
	return 0;
}



double LuaWorker::getRunValue()
{
	if (mutex)
	{
		mutex->Lock();
		double returnValue = runValue;
		mutex->Unlock();
		return returnValue;
	}
	return 0;
}
double LuaWorker::getRotateValue()
{
	if (mutex)
	{
		mutex->Lock();
		double returnValue = rotateValue;
		mutex->Unlock();
		return returnValue;
	}
	return 0;
}
bool LuaWorker::isAllowedToRun()
{
	if (mutex)
	{
		mutex->Lock();
		bool returnValue = allowedToRun;
		mutex->Unlock();
		return returnValue;
	}
	return false;
}

float LuaWorker::getStaminaValue()
{
	if (mutex)
	{
		mutex->Lock();
		float returnValue = stamina;
		mutex->Unlock();
		return returnValue;
	}
	return 0;
}

void LuaWorker::setRunValue(double value)
{
	if (mutex)
	{
		mutex->Lock();
		runValue = value;
		mutex->Unlock();
	}
}
void LuaWorker::setRotateValue(double value)
{
	if (mutex)
	{
		mutex->Lock();
		rotateValue = value;
		mutex->Unlock();
	}
}
void LuaWorker::setAllowedToRun(bool allowed)
{
	if (mutex)
	{
		mutex->Lock();
		allowedToRun = allowed;
		mutex->Unlock();
	}
}

void LuaWorker::setStaminaValue(float value)
{
	if (mutex)
	{
		mutex->Lock();
		stamina = value;
		mutex->Unlock();
	}
}


FDateTime LuaWorker::getLastTick()
{
	return lastTick;
}

void LuaWorker::updateLastTick()
{
	lastTick = FDateTime::Now();
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
			UE_LOG(LogTemp, Warning, TEXT("Couldn't load file: %s\n"), lua_tostring(luaState, -1));
			LuaClose();
		}
		else
		{
			globalMutex->Lock();
			allowedToRun = true;
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
		setAllowedToRun(false);
		if (thread) thread->WaitForCompletion();

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
	lua_pushcfunction(luaState, LuaGetStamina);
	lua_setglobal(luaState, "GetStamina");

	lua_pushcfunction(luaState, LuaAllowedToRun);
	lua_setglobal(luaState, "AllowedToRun");
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
			UE_LOG(LogTemp, Warning, TEXT("error running function %s"), lua_tostring(luaState, -1));
	}
}

