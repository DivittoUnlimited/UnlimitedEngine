#include "DataTables.hpp"

#include "Core/ResourceIdentifiers.hpp"

#include <iostream> // debugging only

extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
}

// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::string  CurrentLuaFile = []()->std::string {
            std::string level = "UNINIT";
            lua_State* L = luaL_newstate();
            luaL_openlibs( L );
            lua_getglobal( L, "debug" );
            lua_getfield( L, -1, "traceback" );
            lua_replace( L, -2 );
            luaL_loadfile( L, "Game/Lua/Application.lua" );
            if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
                luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
                throw( lua_tostring( L, -1 ) );
            }
            lua_getglobal( L, "DefaultLevel" );
            level = lua_tostring( L, -1 );
            lua_close( L );
            return level;
        }();



