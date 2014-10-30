// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Map.h"
#include "../../ThirdParty/lua/Include/lua.h"
#include "../../ThirdParty/lua/Include/lauxlib.h"
#include "../../ThirdParty/lua/Include/lualib.h"
#include "Robot.generated.h"
//#pragma comment(lib, "Lua.lib")

/**
 * 
 */
UCLASS()
class UNREALCUP_API ARobot : public ACharacter
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lua")
	FString luaFile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	int32 team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float stamina;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	void MoveForward(float value);
	void Rotate(float value);


	static TMap<lua_State*, ARobot*> LuaObjectMapping;
protected:
	
	//Tick
	virtual void Tick(float DeltaSeconds) override;

	
	//Lua
	lua_State *luaState;
	void LuaLoad(const char* file);
	void LuaClose();
	void LuaOverridePrint();
	void LuaRegisterFunctions();
	void LuaTick(float DeltaSeconds);

};
