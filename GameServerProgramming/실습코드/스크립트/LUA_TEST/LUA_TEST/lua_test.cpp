#include <iostream>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#pragma comment (lib, "lua53.lib")

using namespace std;

int add_two_c(lua_State* L)
{
	int a = (int)lua_tonumber(L, -2);
	int b = (int)lua_tonumber(L, -1);
	lua_pop(L, 3);
	lua_pushnumber(L, a + b);
	return 1;
}

int main()
{
	const char *buff = "print \"Hello from Lua\"\n";

	lua_State *L = luaL_newstate();	// 루아를 연다.
	luaL_openlibs(L);	// 루아 표준 라이브러리를 연다.
	luaL_loadfile(L, "TEST.LUA");
	int error=lua_pcall(L, 0, 0, 0);
	if (error) {
		cout << "Error : " << lua_tostring(L, -1);
		lua_pop(L, 1);
	}

	lua_register(L, "c_add_two", add_two_c);
	lua_getglobal(L, "add_two");
	lua_pushnumber(L, 100);
	lua_pushnumber(L, 200);
	lua_pcall(L, 2, 1, 0);
	int result = (int)lua_tonumber(L, -1);
	lua_pop(L, 1);

	cout << "result = " << result << endl;
	
	lua_close(L);
	return 0;

}