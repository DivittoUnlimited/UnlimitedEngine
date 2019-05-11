#ifndef DATATABLES_HPP
#define DATATABLES_HPP

#include "ResourceManager.hpp"
#include "ResourceIdentifiers.hpp"
#include "DialogNode.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <vector>
#include <functional>

extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
}
#include <iostream>

class Entity;
class Actor;
class Trigger;
class Item;


struct Direction
{
	Direction(float angle, float distance)
	: angle(angle)
	, distance(distance)
	{
	}

	float angle;
	float distance;
};

struct AnimationData  {
    unsigned int texture;
    sf::Vector2i frameSize;
    unsigned int numFrames;
    sf::Time duration;
    bool isRepeated;
};
struct Limb {
    sf::Vector2f pos;
    float radius;
    sf::Color    color;
};

struct AnimationFrame {
    AnimationFrame( void )
    {
    }
    unsigned int            duration;
    Limb                    leftArm;
    Limb                    rightArm;
    Limb                    leftLeg;
    Limb                    rightLeg;
    std::pair<float, float> spritePos; // relative to it's origin
};

struct ScriptedAnimationData {
    std::vector<AnimationFrame> frames;
};

struct ParticleData
{
    sf::Vector2i position;
    sf::Color    color;
    sf::Time     lifetime;
    int          particleCount;
};

struct ActorData {
    std::vector<Direction> directions;
};

struct WarpData {
    float x; // the new position of the thing being warped.
    float y;
};

struct ItemData {

};

struct ConversationData {
    std::vector<DialogNode> conversationBranches;
};

static std::map<std::string, unsigned int> TextureMap;
static std::map<std::string, unsigned int> FontMap;
static std::map<std::string, unsigned int> SoundEffectMap;
static std::map<std::string, unsigned int> MusicMap;
static std::map<std::string, unsigned int> LayerMap;
static std::map<std::string, unsigned int> ObjectMap;
// Add levels here once everything is working and finally remove Game.lua in place of World1_1.lua
// Which will have everything Game.lua has plus a optional tiledMap path that will be loaded in the World class

static bool loadAssetsLuaFile = []() -> bool {
        lua_State* L = luaL_newstate();
        luaL_openlibs( L );
        lua_getglobal( L, "debug" );
        lua_getfield( L, -1, "traceback" );
        lua_replace( L, -2 );
        luaL_loadfile( L, "Game/Resources/Assets.lua" );
        if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
            luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
            std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
            throw( lua_tostring( L, -1 ) );
        }
        auto getMap = [&]( std::string key ) -> std::map<std::string, unsigned int> {
            std::map<std::string, unsigned int> map;
            lua_getfield( L, -1, key.c_str() );
            if( lua_istable( L, -1 )  ) {
                lua_pushnil( L );
                while( lua_next( L, -2 ) != 0 ) {
                    map.insert( std::pair<std::string, int>( lua_tostring( L , -2 ), lua_tonumber( L , -1 ) ) );
                    lua_pop( L, 1 );
                }
                lua_pop( L ,1 );
            } else throw( "Error reading Assets.lua" );
            return map;
        };
        TextureMap      = getMap( "Textures"     );
        FontMap         = getMap( "Fonts"        );
        SoundEffectMap  = getMap( "SoundEffects" );
        MusicMap        = getMap( "Music"        );
        LayerMap        = getMap( "Layers"       );
        ObjectMap       = getMap( "Objects"      );
        lua_close( L );
        return true;
}( );
static std::map<std::string, std::map<unsigned int, std::string>> MediaFileMap = []( ) -> std::map<std::string, std::map<unsigned int, std::string>>
{
    std::map<std::string, std::map<unsigned int, std::string>> t;

    lua_State* L = luaL_newstate();
    luaL_openlibs( L );

    // call with error checking
    lua_getglobal( L, "debug" );
    lua_getfield( L, -1, "traceback" );
    lua_replace( L, -2 );
    luaL_loadfile( L, "Game/Resources/SourceFileMap.lua" );
    if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
        luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
        std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
        throw( lua_tostring( L, -1 ) );
    }
    if( lua_istable( L, -1 ) )
    {
        lua_pushstring( L, "Textures" );
        lua_gettable( L, -2 );
        if( lua_istable( L, -1 )  )
        {
            std::map<unsigned int, std::string> texSource;
            lua_pushnil( L );
            while( lua_next( L, -2 ) != 0 )
            {
                texSource.insert( std::pair<unsigned int, std::string>( lua_tonumber( L , -2 ), lua_tostring( L , -1 ) ) );
                lua_pop( L, 1 );
            }
            t.insert( std::pair<std::string, std::map<unsigned int, std::string>>( "Textures", texSource ) );
            lua_pop( L ,1 );
        }
        if( lua_istable( L, -1 ) )
        {
            lua_pushstring( L, "Fonts" );
            lua_gettable( L, -2 );
            if( lua_istable( L, -1 )  )
            {
                std::map<unsigned int, std::string> fontSource;
                lua_pushnil( L );
                while( lua_next( L, -2 ) != 0 )
                {
                    fontSource.insert( std::pair<unsigned int, std::string>( lua_tonumber( L , -2 ), lua_tostring( L , -1 ) ) );
                    lua_pop( L, 1 );
                }
                    t.insert( std::pair<std::string, std::map<unsigned int, std::string>>( "Fonts", fontSource ) );
                    lua_pop( L ,1 );
            }
         }
        if( lua_istable( L, -1 ) )
        {
            lua_pushstring( L, "SoundEffects" );
            lua_gettable( L, -2 );
            if( lua_istable( L, -1 )  )
            {
                std::map<unsigned int, std::string> soundSource;
                lua_pushnil( L );
                while( lua_next( L, -2 ) != 0 )
                {
                    soundSource.insert( std::pair<unsigned int, std::string>( lua_tonumber( L , -2 ), lua_tostring( L , -1 ) ) );
                    lua_pop( L, 1 );
                }
                    t.insert( std::pair<std::string, std::map<unsigned int, std::string>>( "SoundEffects", soundSource ) );
                    lua_pop( L ,1 );
            }
         }
    } else std::cout << "Error reading SourceFileMap.lua" << std::endl;
    lua_close( L );

    return t;
}( );
static std::map<std::string, unsigned  int> buildResourceMap( std::string fileName )
{
    std::map<std::string, unsigned int> t;
    lua_State* L = luaL_newstate();
    luaL_openlibs( L );

    // call with error checking
    lua_getglobal( L, "debug" );
    lua_getfield( L, -1, "traceback" );
    lua_replace( L, -2 );
    luaL_loadfile( L, fileName.c_str( ) );
    if( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
        luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
        throw( lua_tostring( L, -1 ) );
    }
    if( lua_istable( L, -1 ) )  // anon table
    {
        lua_getfield( L, -1, "Map" );
        if( lua_istable( L, -1 ) ) // map table
        {
            lua_pushnil( L );  // first key
            while( lua_next( L, -2 ) != 0 ) {
                t.insert( std::pair<std::string, unsigned int>( lua_tostring( L, -2 ), (unsigned int)lua_tointeger( L, -1 )  ) );
                lua_pop( L, 1); // removes 'value'; keeps 'key' for next iteration
            }
            lua_pop( L, 1 ); // remove table
        }
    } else std::cout << "Error reading " << fileName.c_str( ) << " data table" << std::endl;
    lua_close( L );
    return t;
}

static std::map<std::string, unsigned int> ParticleMap          = buildResourceMap( "Game/Resources/Particles.lua" );
static std::map<std::string, unsigned int> ActorMap             = buildResourceMap( "Game/Resources/Actors.lua" );
static std::map<std::string, unsigned int> WarpMap              = buildResourceMap( "Game/Resources/Warps.lua" );
static std::map<std::string, unsigned int> ConversationMap      = buildResourceMap( "Game/Resources/Conversations.lua" );

static std::vector<ActorData> initializeActorData = []() -> std::vector<ActorData> {
        std::vector<ActorData> data( ActorMap.size( ) );
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);
        lua_getglobal( L, "debug" );
        lua_getfield( L, -1, "traceback" );
        lua_replace( L, -2 );
        luaL_loadfile( L, "Game/Resources/Actors.lua" );
        if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
            luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
            std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
            throw( lua_tostring( L, -1 ) );
        }
        if( lua_istable( L, -1 ) ) // Anon table
        {
            for( auto i = ActorMap.begin(); i != ActorMap.end( ); ++i )
            {
                lua_getfield( L, -1, i->first.c_str( ) );
                if( lua_istable( L, -1 ) ) // Actor Definition
                {
                    // "path finding"
                    lua_getfield( L, -1, "path" );
                    if( lua_istable( L, -1 ) )
                    {
                        lua_pushnil( L );
                        while( lua_next(L , -2 ) != 0 )
                        {
                            if( lua_istable( L, -1 ) )
                            {
                                lua_pushnil( L );
                                std::vector<float> v;
                                while( lua_next( L, -2 ) != 0 )
                                {
                                    v.push_back((float)lua_tonumber( L, -1 ));
                                    lua_pop( L, 1 );
                                }
                                //std::cout << "attempting: "<< i->second << ": path {" << a << ", " << d << " }" << std::endl;
                                data[i->second].directions.push_back( Direction( v[0], v[1] ) );
                                lua_pop( L, 1 ); // anon vector
                            } else std::cout << "Error reading actor path data" << std::endl;
                        }
                        lua_pop( L, 1 ); // "path" table
                    }
                }
                lua_pop( L, 1 ); // defintion table
            }
            lua_pop( L, 1 ); // anon table
        }else std::cout << "Error reading Actors.lua" << std::endl;

    lua_close( L );
    return data;
}( );
static std::vector<WarpData> initializeWarpData = []() -> std::vector<WarpData> {
    std::vector<WarpData> data( WarpMap.size( ) );
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_getglobal( L, "debug" );
    lua_getfield( L, -1, "traceback" );
    lua_replace( L, -2 );
    luaL_loadfile( L, "Game/Resources/Warps.lua" );
    if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
        luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
        std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
        throw( lua_tostring( L, -1 ) );
    }
    if( lua_istable( L, -1 ) ) // Anon table
    {
        for( auto i = WarpMap.begin(); i != WarpMap.end(); ++i )
        {
            lua_getfield( L, -1, i->first.c_str( ) );
            if( lua_istable( L, -1 ) ) // Warp Definition
            {
                lua_getfield( L, -1, "x" );
                if( lua_isnumber( L, -1 ) ) data[i->second].x = (float)lua_tonumber( L, -1 );
                else std::cout << "Error loading warp " << i->first.c_str() << "X" << std::endl;
                lua_pop( L, 1 );
                lua_getfield( L, -1, "y" );
                if( lua_isnumber( L, -1 ) ) data[i->second].y = (float)lua_tonumber( L, -1 );
                else std::cout << "Error Loading warp  " << i->first.c_str() << "Y" << std::endl;
                lua_pop( L, 1 );
            }
            lua_pop( L, 1 ); // defintion table
        }
        lua_pop( L, 1 ); // anon table
    }else std::cout << "Error reading Warps.lua" << std::endl;
    lua_close( L );
    return data;
}( );
static std::vector<ParticleData> initializeParticleData = []( ) -> std::vector<ParticleData> {
    std::vector<ParticleData> data( ParticleMap.size( ) );
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);
        lua_getglobal( L, "debug" );
        lua_getfield( L, -1, "traceback" );
        lua_replace( L, -2 );
        luaL_loadfile( L, "Game/Resources/Particles.lua" );
        if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
            luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
            std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
            throw( lua_tostring( L, -1 ) );
        }

        if( lua_istable( L, -1 ) ) // Anon table
        {
            for( auto i = ParticleMap.begin(); i != ParticleMap.end(); ++i )
            {
                lua_getfield( L, -1, i->first.c_str( ) );
                if( lua_istable( L, -1 ) ) // Particle Definition
                {
                    // Get Color Rect
                    sf::Uint8 color[3];
                    lua_getfield( L, -1, "color" );
                    if( lua_istable( L, -1 ) ){
                        unsigned int i = 0;
                        lua_pushnil( L );
                        while( lua_next( L, -2 ) != 0 ) {
                            color[i] = (sf::Uint8)lua_tointeger( L, -1 );
                            ++i;
                            lua_pop( L, 1 );
                        }
                    }
                    data[i->second].color = sf::Color( color[0], color[1], color[2] );
                    lua_pop( L, 1 );

                    lua_getfield( L, -1, "lifetime" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].lifetime = sf::milliseconds( (sf::Uint32)lua_tointeger( L, -1 ) );
                    lua_pop( L, 1 );

                    lua_getfield( L, -1, "particleCount" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].particleCount = (int)lua_tonumber( L, -1 );
                    lua_pop( L, 1 );
                }
                lua_pop( L, 1 ); // defintion table
            }
            lua_pop( L, 1 ); // anon table
        }else std::cout << "Error reading Projectiles.lua" << std::endl;

        lua_close( L );
    return data;
}( ); // initializeParticleData
static std::vector<ConversationData> initializeConversationData = []( ) -> std::vector<ConversationData> {
        std::vector<ConversationData> data( ConversationMap.size( ) );
        lua_State* L = luaL_newstate( );
        luaL_openlibs(L);
        lua_getglobal( L, "debug" );
        lua_getfield( L, -1, "traceback" );
        lua_replace( L, -2 );
        luaL_loadfile( L, "Game/Resources/Conversations.lua" );
        if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
            luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
            std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
            throw( lua_tostring( L, -1 ) );
        }
        if( lua_istable( L, -1 ) ) // Anon table
        {
            for( auto i = ConversationMap.begin( ); i != ConversationMap.end( ); ++i )
            {
                lua_getfield( L, -1, i->first.c_str( ) );
                if( lua_istable( L, -1 ) ) // Conversation Definition
                {
                    // Get Conversation node Array.
                    std::vector<DialogNode> v;

                    lua_pushnil( L );
                    while( lua_next(L , -2 ) != 0 )
                    {
                        std::string text;
                        std::vector<std::pair<std::string, int>> responses;
                        if( lua_istable( L, -1 ) )
                        {
                            lua_getfield( L, -1, "dialog" );
                            if( lua_isstring( L, -1 ) ) text = lua_tostring( L, -1 ); else text = "ERROR1";
                            lua_pop( L, 1 ); // text
                            lua_getfield( L, -1, "responses" );
                            if( lua_istable( L, -1 ) )
                            {
                                // Read in anon response table array
                                lua_pushnil( L );
                                while( lua_next( L, -2 ) != 0 )
                                {
                                    if( lua_istable( L, -1 ) )
                                    {
                                        std::pair<std::string, int> data;

                                        lua_getfield( L, -1, "text" );
                                        if( lua_isstring( L, -1 ) ) data.first = lua_tostring( L, -1 ); else data.first = "ERROR2";
                                        lua_pop( L, 1 ); // text

                                        lua_getfield( L, -1, "link" );
                                        if( lua_isnumber( L, -1 ) ) data.second = (int)lua_tonumber( L, -1 );
                                        lua_pop( L, 1 ); // link

                                        responses.push_back( data );
                                    }
                                    lua_pop( L, 1 ); // data pair object.
                                }
                            }
                            else std::cout << "ERROR reading responses for: " << i->first.c_str( ) << std::endl;
                            lua_pop( L, 1 ); // Responses table
                        } else std::cout << "Error reading ConversationData" << std::endl;
                        lua_pop( L, 1 ); // DialogNode
                        v.push_back( DialogNode( text, responses ) );
                    }

                    data[i->second].conversationBranches = v;

                    lua_pop( L, 1 ); // nil value to traverse convo Links
                }
                lua_pop( L, 1 ); // defintion table
            }

        }else std::cout << "Error reading Conversations.lua" << std::endl;
        lua_pop( L, 1 ); // anon table
        lua_close( L );
        return data;
}( ); // initializeConversationData

#endif // DATATABLES_HPP




































