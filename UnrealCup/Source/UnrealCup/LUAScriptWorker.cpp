// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealCup.h"
#include "LUAScriptWorker.h"

//Static varaibles:
TMap<lua_State*, LUAScriptWorker*> LUAScriptWorker::LuaObjectMapping;
//FPlatformProcess::FSemaphore* LUAScriptWorker::globalMutex = FPlatformProcess::NewInterprocessSynchObject("Global Lua Mutex", true);
FCriticalSection LUAScriptWorker::globalMutex = FCriticalSection();


static int32 LuaGetTeamId(lua_State* L)
{
	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		lua_pushinteger(L, worker->getTeamId());
	}
	else
	{
		lua_pushinteger(L, -1);
	}

	return 1;
}

static int32 LuaGetTimePlayed(lua_State* L)
{
	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		lua_pushinteger(L, worker->getTimePlayed());
	}
	else
	{
		lua_pushinteger(L, -1);
	}

	return 1;
}

static int32 LuaIsKickoff(lua_State* L)
{
	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		lua_pushinteger(L, worker->isKickoff());
	}
	else
	{
		lua_pushinteger(L, -1);
	}

	return 1;
}


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

static int32 LuaHasBall(lua_State* L)
{
	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		lua_pushboolean(L, worker->hasBall());
	}
	else
	{
		lua_pushboolean(L, false);
	}
	return 1;
}

static int32 LuaMoveTo(lua_State* L)
{
	double x = lua_tonumber(L, 1);
	double y = lua_tonumber(L, 2);
	float speed = lua_tonumber(L, 3);

	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		worker->moveTo(x, y, speed);
	}

	return 0;
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

static int32 LuaKick(lua_State* L)
{
	double x = lua_tonumber(L, 1);
	double y = lua_tonumber(L, 2);
	double z = lua_tonumber(L, 3);
	double force = lua_tonumber(L, 4);

	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		FVector direction = FVector(x, y, z);
		worker->kick(direction, force);
	}
	return 0;
}

static int32 LuaStopBall(lua_State* L)
{
	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		worker->stopBall();
	}

	return 0;
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

static int32 LuaGetVisiblePlayers(lua_State* L)
{
	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		TArray<RobotDataTypes::PlayerLocation>* locations = worker->getVisiblePlayersAbsolute();
		if (locations != nullptr)
		{
			lua_newtable(L);
			for (int i = 0; i < locations->Num(); i++)
			{
				lua_newtable(L);
				lua_pushnumber(L, (*locations)[i].teamId);
				lua_rawseti(L, -2, 1);
				lua_pushnumber(L, (*locations)[i].playerId);
				lua_rawseti(L, -2, 2);
				lua_pushnumber(L, (*locations)[i].position->X);
				lua_rawseti(L, -2, 3);
				lua_pushnumber(L, (*locations)[i].position->Y);
				lua_rawseti(L, -2, 4);

				lua_rawseti(L, -2, i + 1);

				delete (*locations)[i].position;
			}
			delete locations;
			return 1;
		}

		
	}

	//No data available return Null array
	lua_newtable(L);

	lua_newtable(L);
	lua_pushnumber(L, 0);
	lua_rawseti(L, -2, 1);
	lua_pushnumber(L, 0);
	lua_rawseti(L, -2, 2);
	lua_pushnumber(L, 0);
	lua_rawseti(L, -2, 3);
	lua_pushnumber(L, 0);
	lua_rawseti(L, -2, 4);

	lua_rawseti(L, -2, 1);
	return 1;
}


static int32 LuaGetBallPosition(lua_State* L)
{
	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		FVector pos = worker->getBallPosition();

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
	return 3;
}




static int32 LuaGetGoal1Position(lua_State* L)
{
	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		FVector pos = worker->getGoal1Position();

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
	return 3;
}

static int32 LuaGetGoal2Position(lua_State* L)
{
	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		FVector pos = worker->getGoal2Position();

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
	return 3;
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
		if (calcTime > 5)
		{
			if (calcTime > 10)calcTime = 10;
			FPlatformProcess::Sleep(calcTime / 100);
			worker->updateLastTick();
		}
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





static int32 LuaSpeak(lua_State* L)
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
	//UE_LOG(LogTemp, Warning, TEXT("Speak: %s"), *Message);
	
	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		worker->speak(Message);
	}

	return 0;
}


static int32 LuaListen(lua_State* L)
{
	FString text = FString("");

	LUAScriptWorker* worker = LUAScriptWorker::getLuaWorker(L);
	if (worker)
	{
		text = worker->getSpoken();
	}
	lua_pushfstring(L, TCHAR_TO_ANSI(*text));

	return 1;
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
	globalMutex.Lock();
	LUAScriptWorker* worker = LuaObjectMapping.FindRef(L);
	globalMutex.Unlock();

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
			const char* msg = lua_tostring(luaState, -1);
			if (msg == NULL)
			{
				if (status == LUA_ERRSYNTAX)
				{
					msg = "SYNTAX ERROR";
				}
				else
				{
					msg = "UNKOWN ERROR";
				}

			}
			//UE_LOG(LogTemp, Warning, TEXT("Couldn't load '%s': Errorcode: %d, %s\n"), file, status, msg);
			FString error = FString("Couldn't load KI: ");
			//error.Append(file);
			//error.Append("': Errorcode: ");
			//error.AppendInt(status);
			//error.Append(", ");
			error.Append(msg);

			/*
			GEngine->bEnableOnScreenDebugMessages = true;
			GEngine->bEnableOnScreenDebugMessagesDisplay = true;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, error);
			*/
			
			
			
			//FGenericPlatformMisc::MessageBoxExt(EAppMsgType::Ok, ANSI_TO_TCHAR(msg), TEXT("Couldn't load KI"));
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(msg));

			freeLuaScript();
		}
		else
		{
			globalMutex.Lock();
			LuaObjectMapping.Add(luaState, this);
			globalMutex.Unlock();
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
			globalMutex.Lock();
			LuaObjectMapping.Remove(luaState);
			globalMutex.Unlock();
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
	lua_pushcfunction(luaState, LuaGetTimePlayed);
	lua_setglobal(luaState, "TimePlayed");

	lua_pushcfunction(luaState, LuaGetTeamId);
	lua_setglobal(luaState, "GetTeamId");

	lua_pushcfunction(luaState, LuaIsKickoff);
	lua_setglobal(luaState, "IsKickoff");

	lua_pushcfunction(luaState, LuaHasBall);
	lua_setglobal(luaState, "hasBall");

	lua_pushcfunction(luaState, LuaMoveForward);
	lua_setglobal(luaState, "MoveForward");

	lua_pushcfunction(luaState, LuaMoveTo);
	lua_setglobal(luaState, "MoveTo");

	lua_pushcfunction(luaState, LuaRotate);
	lua_setglobal(luaState, "Rotate");

	lua_pushcfunction(luaState, LuaKick);
	lua_setglobal(luaState, "Kick");

	lua_pushcfunction(luaState, LuaStopBall);
	lua_setglobal(luaState, "StopBall");

	lua_pushcfunction(luaState, LuaGetOwnLocation);
	lua_setglobal(luaState, "GetOwnLocation");

	lua_pushcfunction(luaState, LuaGetBallPosition);
	lua_setglobal(luaState, "GetBallPosition");

	lua_pushcfunction(luaState, LuaGetVisiblePlayers);
	lua_setglobal(luaState, "GetVisiblePlayers");

	lua_pushcfunction(luaState, LuaGetGoal1Position);
	lua_setglobal(luaState, "GetGoal1Position");

	lua_pushcfunction(luaState, LuaGetGoal2Position);
	lua_setglobal(luaState, "GetGoal2Position");

	lua_pushcfunction(luaState, LuaGetStamina);
	lua_setglobal(luaState, "GetStamina");

	lua_pushcfunction(luaState, LuaSpeak);
	lua_setglobal(luaState, "Speak");

	lua_pushcfunction(luaState, LuaListen);
	lua_setglobal(luaState, "Listen");

	lua_pushcfunction(luaState, LuaAllowedToRun);
	lua_setglobal(luaState, "AllowedToRun");
}


//Call Tick(float DeltaSeconds) funtion in Lua script
uint32 LUAScriptWorker::Run()
{
	RobotScriptWorker::Run();

	//UE_LOG(LogTemp, Warning, TEXT("LUA RUN!!!"));

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



