#include "DataTables.hpp"

#include "ResourceIdentifiers.hpp"

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
            luaL_loadfile( L, "Game/Application.lua" );
            if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
                luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
                throw( lua_tostring( L, -1 ) );
            }
            lua_getglobal( L, "DefaultLevel" );
            level = lua_tostring( L, -1 );
            lua_close( L );
            return level;
        }();


std::vector<PickupData> initializePickupData()
{
    // std::vector<PickupData> data(Pickup::TypeCount);



    /*
 * The Lua way....
 * currently not working
 *
 *
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_register( L, "Pickup", []( lua_State* L ) {
        PickupData* pd = (PickupData*)lua_newuserdata( L, sizeof( PickupData ) );
        new (pd) PickupData;
        pd->texture = (unsigned int)lua_tonumber( L, 1 );
        // pd->action = lua_touserdata( L, 2 );
        return 1;
    });
*/

    /*
    data[Pickup::HealthRefill].texture =  TextureMap.at( "Entities" );
    data[Pickup::HealthRefill].texRect =  sf::IntRect(0, 64, 40, 40);
    data[Pickup::HealthRefill].action = [] (Aircraft& a) { a.repair(10); };
		
    data[Pickup::FireSpread].texture = TextureMap.at( "Entities" );
    data[Pickup::FireSpread].texRect = sf::IntRect(80, 64, 40, 40);
    data[Pickup::FireSpread].action = [] (Aircraft& a) { a.increaseSpread(); };
	
    data[Pickup::Ally].texture = TextureMap.at( "Ally" );
    data[Pickup::Ally].texRect = sf::IntRect( 0, 0, 24, 48 );
    data[Pickup::Ally].action = [] (Aircraft& a) { a.increaseTroopCount(); };

    data[Pickup::Fuel].texture = TextureMap.at( "Fuel" );
    data[Pickup::Fuel].texRect = sf::IntRect( 0, 0, 32, 48 );
    data[Pickup::Fuel].action = [] ( Aircraft& a ) { a.increaseFuel(); };
    */
    //return data;
}



