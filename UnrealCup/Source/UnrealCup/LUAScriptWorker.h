// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../../ThirdParty/lua/Include/lua.h"
#include "../../ThirdParty/lua/Include/lauxlib.h"
#include "../../ThirdParty/lua/Include/lualib.h"
#include "RobotScriptWorker.h"

/**
 * 
 */
class UNREALCUP_API LUAScriptWorker : public RobotScriptWorker
{
private:
	static TMap<lua_State*, LUAScriptWorker*> LuaObjectMapping;
	void loadLuaScript(const char* file);
	void freeLuaScript();

protected:
	lua_State *luaState;
	static FPlatformProcess::FSemaphore* globalMutex;

	

	void overridePrint();
	void registerFunctions();

	virtual uint32 Run();
	virtual void Stop();

public:
	LUAScriptWorker(RobotControl* robotController, FString file);
	~LUAScriptWorker();

	static LUAScriptWorker* getLuaWorker(lua_State* L);
};
