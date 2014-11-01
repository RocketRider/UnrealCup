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

	

	//Lua
	lua_State *luaState;
	void LuaLoad(const char* file);
	void LuaClose();
	void LuaOverridePrint();
	void LuaRegisterFunctions();
	void LuaRun();

public:

	static LuaWorker* LuaInit(ARobot* robot, const char* file);

	static TMap<lua_State*, LuaWorker*> LuaObjectMapping;

	ARobot* robot;

	// Begin FRunnable interface.
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	// End FRunnable interface

	LuaWorker(ARobot* robot, const char* file);
	~LuaWorker();
};
