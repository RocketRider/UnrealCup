// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../ThirdParty/lua/Include/lua.h"
#include "../../ThirdParty/lua/Include/lauxlib.h"
#include "../../ThirdParty/lua/Include/lualib.h"
#include "Robot.h"


//Forward declaration
//class ARobot;

/**
 * 
 */
class UNREALCUP_API LuaWorker : public FRunnable
{
private:
	/** Thread to run the worker FRunnable on */
	FRunnableThread* thread;

	const char* luafile;

	double ownX, ownY, ownZ;

	double runValue;
	double rotateValue;
	bool allowedToRun;
	FDateTime lastTick;


	static TMap<lua_State*, LuaWorker*> LuaObjectMapping;
	//TODO: Multiplatform 
	FPlatformProcess::FSemaphore* mutex;
	static FPlatformProcess::FSemaphore* globalMutex;

	ARobot* robot;

	//Lua
	lua_State *luaState;
	void LuaLoad(const char* file);
	void LuaClose();
	void LuaOverridePrint();
	void LuaRegisterFunctions();
	void LuaRun();



public:

	static LuaWorker* LuaInit(ARobot* robot, const char* file);
	static LuaWorker* getLuaWorker(lua_State* L);

	void setOwnLocation(double x, double y, double z);
	double getOwnX();
	double getOwnY();
	double getOwnZ();

	double getRunValue();
	double getRotateValue();
	bool isAllowedToRun();

	void setRunValue(double value);
	void setRotateValue(double value);
	void setAllowedToRun(bool allowed);

	FDateTime getLastTick();
	void updateLastTick();

	


	// Begin FRunnable interface.
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	// End FRunnable interface

	LuaWorker(ARobot* robot, const char* file);
	~LuaWorker();
};
