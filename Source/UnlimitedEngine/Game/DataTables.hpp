#ifndef DATATABLES_HPP
#define DATATABLES_HPP

#include "Core/ResourceManager.hpp"
#include "Core/ResourceIdentifiers.hpp"
#include "Core/DialogNode.hpp"
#include "Core/Category.hpp"
#include "Game/Dice.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <array>
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

// left over from western rpg game
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

// left over from western rpg game
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

// Left over from a western rpg game
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
    std::vector<int> portrait; // lua identifier // matches up with corresponding link in convo
    std::vector<sf::Vector2f> portraitPosition; // index will line up with other two vectors
};

struct StatModifier
{
    std::string stat;
    Dice power;
    unsigned int duration;
};

struct AbilityData
{
    unsigned int coolDown;
    unsigned int range;
    bool hasRotation;
    std::vector<std::vector<sf::Vector2i>> AOE;
    std::vector<StatModifier> targetMods;
    std::vector<StatModifier> userMods;
};

struct UnitTypeData {
    Category::Type category;
    std::string type;
    int attack;
    int armour;
    int speed;
    unsigned int perception;
    int health;
    int morale;
    int stamina;
    std::string textureID;
    sf::Rect<int> textureRect;
    std::vector<std::string> abilities;
};

/// \brief The BuildingData struct
/// Probably obsolete since the only "building" type in the game is the spawn points
/// anymore :P
struct BuildingData {
    Category::Type category;
    std::string type;
    unsigned int goldPerTurn;
    std::string textureID;
};

static std::map<std::string, unsigned int> TextureMap;
static std::map<std::string, unsigned int> FontMap;
static std::map<std::string, unsigned int> SoundEffectMap;
static std::map<std::string, unsigned int> MusicMap;
static std::map<std::string, unsigned int> LayerMap;
static std::map<std::string, unsigned int> ObjectMap;
static std::map<std::string, unsigned int> LevelMap;

static bool loadAssetsLuaFile = []() -> bool {
        lua_State* L = luaL_newstate();
        luaL_openlibs( L );
        lua_getglobal( L, "debug" );
        lua_getfield( L, -1, "traceback" );
        lua_replace( L, -2 );
        luaL_loadfile( L, "Game/Lua/Resources/Assets.lua" );
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
        LevelMap        = getMap( "Maps"         );
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
    luaL_loadfile( L, "Game/Lua/Resources/SourceFileMap.lua" );
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
        if( lua_istable( L, -1 ) )
        {
            lua_pushstring( L, "Maps" );
            lua_gettable( L, -2 );
            if( lua_istable( L, -1 )  )
            {
                std::map<unsigned int, std::string> mapSource;
                lua_pushnil( L );
                while( lua_next( L, -2 ) != 0 )
                {
                    mapSource.insert( std::pair<unsigned int, std::string>( lua_tonumber( L , -2 ), lua_tostring( L , -1 ) ) );
                    lua_pop( L, 1 );
                }
                    t.insert( std::pair<std::string, std::map<unsigned int, std::string>>( "Maps", mapSource ) );
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
                t.insert( std::pair<std::string, unsigned int>( lua_tostring( L, -2 ), static_cast<unsigned int>( lua_tointeger( L, -1 )  ) ) );
                lua_pop( L, 1); // removes 'value'; keeps 'key' for next iteration
            }
            lua_pop( L, 1 ); // remove table
        }
    } else std::cout << "Error reading " << fileName.c_str( ) << " data table" << std::endl;
    lua_close( L );
    return t;
}

static std::map<std::string, unsigned int> ConversationMap      = buildResourceMap( "Game/Lua/Resources/Conversations.lua" );
static std::map<std::string, unsigned int> ParticleMap          = buildResourceMap( "Game/Lua/Resources/Particles.lua" );
static std::map<std::string, unsigned int> ActorMap             = buildResourceMap( "Game/Lua/Resources/Actors.lua" );
static std::map<std::string, unsigned int> WarpMap              = buildResourceMap( "Game/Lua/Resources/Warps.lua" );

// Tactics tribe Maps
static std::map<std::string, unsigned int> TerrainTypeMap       = buildResourceMap( "Game/Lua/TerrainTypes.lua" );
static std::map<std::string, unsigned int> UnitTypeMap          = buildResourceMap( "Game/Lua/UnitTypes.lua" );
static std::map<std::string, unsigned int> AbilityMap           = buildResourceMap( "Game/Lua/Abilities.lua" );
static std::map<std::string, unsigned int> BuildingTypeMap      = buildResourceMap( "Game/Lua/Buildings.lua" );

static std::vector<WarpData> initializeWarpData = []() -> std::vector<WarpData> {
    std::vector<WarpData> data( WarpMap.size( ) );
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_getglobal( L, "debug" );
    lua_getfield( L, -1, "traceback" );
    lua_replace( L, -2 );
    luaL_loadfile( L, "Game/Lua/Resources/Warps.lua" );
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
        luaL_loadfile( L, "Game/Lua/Resources/Particles.lua" );
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
        luaL_loadfile( L, "Game/Lua/Resources/Conversations.lua" );
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
                    int portrait = 0;
                    sf::Vector2f position;

                    lua_pushnil( L );
                    while( lua_next( L, -2 ) != 0 )
                    {
                        std::string text;
                        std::vector<std::pair<std::string, int>> responses;
                        if( lua_istable( L, -1 ) )
                        {
                            lua_getfield( L, -1, "portrait" );
                            if( lua_isnumber( L, -1 ) ) portrait = static_cast<int>( lua_tointeger( L, -1 ) ); else portrait = 0;
                            lua_pop( L, 1 ); // portrait

                            lua_getfield( L, -1, "x" );
                            if( lua_isnumber( L, -1 ) ) position.x = static_cast<unsigned int>( lua_tointeger( L, -1 ) ); else position.x = 0; // center
                            lua_pop( L, 1 ); // y

                            lua_getfield( L, -1, "y" );
                            if( lua_isnumber( L, -1 ) ) position.y = static_cast<unsigned int>( lua_tointeger( L, -1 ) ); else position.y = 0; // center
                            lua_pop( L, 1 ); // y

                            // =========================
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
                                        if( lua_isnumber( L, -1 ) ) data.second = static_cast<int>( lua_tonumber( L, -1 ) );
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
                        data[i->second].portrait.push_back( portrait );
                        data[i->second].portraitPosition.push_back( position );
                    }

                    data[i->second].conversationBranches = v;


                    //lua_pop( L, 1 ); // nil value to traverse convo Links
                }
                lua_pop( L, 1 ); // defintion table
            }

        }else std::cout << "Error reading Conversations.lua" << std::endl;
        lua_pop( L, 1 ); // anon table
        lua_close( L );
        return data;
}( ); // initializeConversationData
static std::vector<AbilityData> initializeAbilityData = []( ) -> std::vector<AbilityData> {
        std::vector<AbilityData> data( AbilityMap.size( ) );
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);
        lua_getglobal( L, "debug" );
        lua_getfield( L, -1, "traceback" );
        lua_replace( L, -2 );
        luaL_loadfile( L, "Game/Lua/Abilities.lua" );
        if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
            luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
            std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
            throw( lua_tostring( L, -1 ) );
        }
        if( lua_istable( L, -1 ) ) // Anon table
        {
            for( auto i = AbilityMap.begin(); i != AbilityMap.end(); ++i )
            {
                lua_getfield( L, -1, i->first.c_str( ) );
                if( lua_istable( L, -1 ) ) // Ability Definition
                {
                    lua_getfield( L, -1, "coolDown" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].coolDown = static_cast<int>( lua_tonumber( L, -1 ) );
                    else std::cout << "Error loading Abilities " << i->first.c_str() << "coolDown" << std::endl;
                    lua_pop( L, 1 ); // CoolDown
                    lua_getfield( L, -1, "range" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].range = static_cast<int>( lua_tonumber( L, -1 ) );
                    else std::cout << "Error loading Abilities " << i->first.c_str() << "range" << std::endl;
                    lua_pop( L, 1 ); // range
                    lua_getfield( L, -1, "hasRotation" );
                    if( lua_isboolean( L, -1 ) ) data[i->second].hasRotation = lua_toboolean( L, -1 );
                    else std::cout << "Error loading Abilities " << i->first.c_str() << "hasRotation" << std::endl;
                    lua_pop( L, 1 ); // hasRotation

                    if( !data[i->second].hasRotation )
                    {
                        // MOD THIS TO BE AOE IF APPLICABLE
                        lua_getfield( L, -1, "AOE" );
                        if( lua_istable( L, -1 ) )
                        {
                            data[i->second].AOE.push_back( std::vector<sf::Vector2i>( ) );
                            // build int pairs vector
                            lua_pushnil( L );
                            while( lua_next( L, -2 ) != 0 )
                            {
                                if( lua_istable( L, -1 ) )
                                {
                                    int x = 0;
                                    int y = 0;
                                    lua_getfield( L, -1, "x" );
                                    if( lua_isnumber( L, -1 ) ) x = static_cast<int>( lua_tonumber( L, -1 ) );
                                    else std::cout << "Error loading Ability AOE" <<  std::endl;
                                    lua_pop( L, 1 );
                                    lua_getfield( L, -1, "y" );
                                    if( lua_isnumber( L, -1 ) ) y = static_cast<int>( lua_tonumber( L, -1 ) );
                                    else std::cout << "Error loading Ability AOE" <<  std::endl;
                                    lua_pop( L, 1 );
                                    data[i->second].AOE.back().push_back( sf::Vector2i( x, y ) );
                                }
                                lua_pop( L, 1 );
                            }
                        }
                        else std::cout << "Error Reading Ability AOE" << std::endl;
                        lua_pop( L, 1 ); // AOE
                    }
                    else
                    {
                        // MOD THIS TO BE AOE IF APPLICABLE
                        lua_getfield( L, -1, "AOE" );
                        if( lua_istable( L, -1 ) )
                        {
                            data[i->second].AOE.push_back( std::vector<sf::Vector2i>( ) );
                            lua_getfield( L, -1, "north" );
                            if( lua_istable( L, -1 ) )
                            {
                                // build int pairs vector
                                lua_pushnil( L );
                                while( lua_next( L, -2 ) != 0 )
                                {
                                    if( lua_istable( L, -1 ) )
                                    {
                                        int x = 0;
                                        int y = 0;
                                        lua_getfield( L, -1, "x" );
                                        if( lua_isnumber( L, -1 ) ) x = static_cast<int>( lua_tonumber( L, -1 ) );
                                        else std::cout << "Error loading Ability AOE" <<  std::endl;
                                        lua_pop( L, 1 );
                                        lua_getfield( L, -1, "y" );
                                        if( lua_isnumber( L, -1 ) ) y = static_cast<int>( lua_tonumber( L, -1 ) );
                                        else std::cout << "Error loading Ability AOE" <<  std::endl;
                                        lua_pop( L, 1 );
                                        data[i->second].AOE.back().push_back( sf::Vector2i( x, y ) );
                                    }
                                    lua_pop( L, 1 );
                                }
                            }
                            lua_pop( L, 1 ); // north
                            data[i->second].AOE.push_back( std::vector<sf::Vector2i>( ) );
                            lua_getfield( L, -1, "east" );
                            if( lua_istable( L, -1 ) )
                            {
                                // build int pairs vector
                                lua_pushnil( L );
                                while( lua_next( L, -2 ) != 0 )
                                {
                                    if( lua_istable( L, -1 ) )
                                    {
                                        int x = 0;
                                        int y = 0;
                                        lua_getfield( L, -1, "x" );
                                        if( lua_isnumber( L, -1 ) ) x = static_cast<int>( lua_tonumber( L, -1 ) );
                                        else std::cout << "Error loading Ability AOE" <<  std::endl;
                                        lua_pop( L, 1 );
                                        lua_getfield( L, -1, "y" );
                                        if( lua_isnumber( L, -1 ) ) y = static_cast<int>( lua_tonumber( L, -1 ) );
                                        else std::cout << "Error loading Ability AOE" <<  std::endl;
                                        lua_pop( L, 1 );
                                        data[i->second].AOE.back().push_back( sf::Vector2i( x, y ) );
                                    }
                                    lua_pop( L, 1 );
                                }
                            }
                            lua_pop( L, 1 ); // east
                            data[i->second].AOE.push_back( std::vector<sf::Vector2i>( ) );
                            lua_getfield( L, -1, "south" );
                            if( lua_istable( L, -1 ) )
                            {
                                // build int pairs vector
                                lua_pushnil( L );
                                while( lua_next( L, -2 ) != 0 )
                                {
                                    if( lua_istable( L, -1 ) )
                                    {
                                        int x = 0;
                                        int y = 0;
                                        lua_getfield( L, -1, "x" );
                                        if( lua_isnumber( L, -1 ) ) x = static_cast<int>( lua_tonumber( L, -1 ) );
                                        else std::cout << "Error loading Ability AOE" <<  std::endl;
                                        lua_pop( L, 1 );
                                        lua_getfield( L, -1, "y" );
                                        if( lua_isnumber( L, -1 ) ) y = static_cast<int>( lua_tonumber( L, -1 ) );
                                        else std::cout << "Error loading Ability AOE" <<  std::endl;
                                        lua_pop( L, 1 );
                                        data[i->second].AOE.back().push_back( sf::Vector2i( x, y ) );
                                    }
                                    lua_pop( L, 1 );
                                }
                            }
                            lua_pop( L, 1 ); // south
                            data[i->second].AOE.push_back( std::vector<sf::Vector2i>( ) );
                            lua_getfield( L, -1, "west" );
                            if( lua_istable( L, -1 ) )
                            {
                                // build int pairs vector
                                lua_pushnil( L );
                                while( lua_next( L, -2 ) != 0 )
                                {
                                    if( lua_istable( L, -1 ) )
                                    {
                                        int x = 0;
                                        int y = 0;
                                        lua_getfield( L, -1, "x" );
                                        if( lua_isnumber( L, -1 ) ) x = static_cast<int>( lua_tonumber( L, -1 ) );
                                        else std::cout << "Error loading Ability AOE" <<  std::endl;
                                        lua_pop( L, 1 );
                                        lua_getfield( L, -1, "y" );
                                        if( lua_isnumber( L, -1 ) ) y = static_cast<int>( lua_tonumber( L, -1 ) );
                                        else std::cout << "Error loading Ability AOE" <<  std::endl;
                                        lua_pop( L, 1 );
                                        data[i->second].AOE.back().push_back( sf::Vector2i( x, y ) );
                                    }
                                    lua_pop( L, 1 );
                                }
                            }
                            lua_pop( L, 1 ); // west

                        }
                        else std::cout << "Error Reading Ability AOE" << std::endl;
                        lua_pop( L, 1 ); // AOE
                    }

                    lua_getfield( L, -1, "targetMods" );
                    if( lua_istable( L, -1 ) )
                    {
                        lua_pushnil( L );
                        while( lua_next( L, -2 ) != 0 )
                        {
                            if( lua_istable( L, -1 ) )
                            {
                                data[i->second].targetMods.push_back( StatModifier( ) );
                                lua_getfield( L, -1, "stat" );
                                if( lua_isstring( L, -1 ) ) data[i->second].targetMods.back().stat = lua_tostring( L, -1 );
                                else std::cout << "Error loading Ability TargetMods" <<  std::endl;
                                lua_pop( L, 1 );
                                lua_getfield( L, -1, "power" );
                                if( lua_isstring( L, -1 ) ) data[i->second].targetMods.back().power = Dice( lua_tostring( L, -1 ) );
                                else std::cout << "Error loading Ability TargetMods" <<  std::endl;
                                lua_pop( L, 1 );
                                lua_getfield( L, -1, "duration" );
                                if( lua_isnumber( L, -1 ) ) data[i->second].targetMods.back().duration = static_cast<int>( lua_tonumber( L, -1 ) );
                                else std::cout << "Error loading Ability TargetMods" <<  std::endl;
                                lua_pop( L, 1 );
                            }
                            lua_pop( L, 1 );
                        }
                    }else std::cout << "Error reading TargetMods of ability." << std::endl;
                    lua_pop( L, 1 ); // // TargetMods

                    lua_getfield( L, -1, "userMods" );
                    if( lua_istable( L, -1 ) )
                    {
                        lua_pushnil( L );
                        while( lua_next( L, -2 ) != 0 )
                        {
                            if( lua_istable( L, -1 ) )
                            {
                                data[i->second].userMods.push_back( StatModifier( ) );
                                lua_getfield( L, -1, "stat" );
                                if( lua_isstring( L, -1 ) ) data[i->second].userMods.back().stat = lua_tostring( L, -1 );
                                else std::cout << "Error loading Ability UserMods" <<  std::endl;
                                lua_pop( L, 1 );
                                lua_getfield( L, -1, "power" );
                                if( lua_isstring( L, -1 ) ) data[i->second].userMods.back().power = Dice( lua_tostring( L, -1 ) );
                                else std::cout << "Error loading Ability UserMods" <<  std::endl;
                                lua_pop( L, 1 );
                                lua_getfield( L, -1, "duration" );
                                if( lua_isnumber( L, -1 ) ) data[i->second].userMods.back().duration = static_cast<int>( lua_tonumber( L, -1 ) );
                                else std::cout << "Error loading Ability UserMods" <<  std::endl;
                                lua_pop( L, 1 );
                            }
                            lua_pop( L, 1 );
                        }
                    }else std::cout << "Error reading UserMods of ability." << std::endl;
                    lua_pop( L, 1 ); // // UserMods
                }else std::cout << "ERROR Reading Ability" << std::endl;
                lua_pop( L, 1 ); // Ability Definition
            }
        }else std::cout << "Error reading Abilities.lua" << std::endl;
        lua_pop( L, 1 ); // anon table
        lua_close( L );
        return data;
}(  ); // initializeAbilityData
static std::vector<UnitTypeData> initializeUnitTypeData = []( ) -> std::vector<UnitTypeData> {
        std::vector<UnitTypeData> data( UnitTypeMap.size( ) );
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);
        lua_getglobal( L, "debug" );
        lua_getfield( L, -1, "traceback" );
        lua_replace( L, -2 );
        luaL_loadfile( L, "Game/Lua/UnitTypes.lua" );
        if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
            luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
            std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
            throw( lua_tostring( L, -1 ) );
        }
        if( lua_istable( L, -1 ) ) // Anon table
        {
            for( auto i = UnitTypeMap.begin(); i != UnitTypeMap.end(); ++i )
            {
                lua_getfield( L, -1, i->first.c_str( ) );
                if( lua_istable( L, -1 ) ) // UnitType Definition
                {
                    lua_getfield( L, -1, "class" );
                    if( lua_isstring( L, -1 ) ) data[i->second].type = lua_tostring( L, -1 );
                    else std::cout << "Error loading UnitType " << i->first.c_str() << "type" << std::endl;
                    lua_pop( L, 1 );

                    lua_getfield( L, -1, "attack" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].attack = static_cast<int>( lua_tonumber( L, -1 ) );
                    else std::cout << "Error loading UnitType " << i->first.c_str() << "strength" << std::endl;
                    lua_pop( L, 1 );
                    lua_getfield( L, -1, "speed" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].speed = static_cast<int>( lua_tonumber( L, -1 ) );
                    else std::cout << "Error loading UnitType " << i->first.c_str() << "speed" << std::endl;
                    lua_pop( L, 1 );
                    lua_getfield( L, -1, "health" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].health = static_cast<int>( lua_tonumber( L, -1 ) );
                    else std::cout << "Error loading UnitType " << i->first.c_str() << "Health" << std::endl;
                    lua_pop( L, 1 );
                    lua_getfield( L, -1, "armour" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].armour = static_cast<int>( lua_tonumber( L, -1 ) );
                    else std::cout << "Error loading UnitType " << i->first.c_str() << "armour" << std::endl;
                    lua_pop( L, 1 );
                    lua_getfield( L, -1, "perception" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].perception = static_cast<unsigned int>( lua_tonumber( L, -1 ) );
                    else std::cout << "Error loading UnitType " << i->first.c_str() << "perception" << std::endl;
                    lua_pop( L, 1 );
                    lua_getfield( L, -1, "morale" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].morale = static_cast<unsigned int>( lua_tonumber( L, -1 ) );
                    else std::cout << "Error loading UnitType " << i->first.c_str() << "morale" << std::endl;
                    lua_pop( L, 1 );
                    lua_getfield( L, -1, "stamina" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].stamina = static_cast<unsigned int>( lua_tonumber( L, -1 ) );
                    else std::cout << "Error loading UnitType " << i->first.c_str() << "stamina" << std::endl;
                    lua_pop( L, 1 );
                    lua_getfield( L, -1, "texture" );
                    if( lua_isstring( L, -1 ) ) data[i->second].textureID = lua_tostring( L, -1 );
                    else std::cout << "Error loading UnitType " << i->first.c_str() << "texture" << std::endl;
                    lua_pop( L, 1 );
                    lua_getfield( L, -1, "textureRect" );
                    if( lua_istable( L, -1 ) )
                    {
                        lua_getfield( L, -1, "top" );
                        if( lua_isnumber( L, -1 ) ) data[i->second].textureRect.top = static_cast<int>( lua_tonumber( L, -1 ) );
                        else std::cout << "Error loading UnitType " << i->first.c_str() << "TextureRect" << std::endl;
                        lua_pop( L, 1 );
                        lua_getfield( L, -1, "left" );
                        if( lua_isnumber( L, -1 ) ) data[i->second].textureRect.left = static_cast<int>( lua_tonumber( L, -1 ) );
                        else std::cout << "Error loading UnitType " << i->first.c_str() << "TextureRect" << std::endl;
                        lua_pop( L, 1 );
                        lua_getfield( L, -1, "width" );
                        if( lua_isnumber( L, -1 ) ) data[i->second].textureRect.width = static_cast<int>( lua_tonumber( L, -1 ) );
                        else std::cout << "Error loading UnitType " << i->first.c_str() << "TextureRect" << std::endl;
                        lua_pop( L, 1 );
                        lua_getfield( L, -1, "height" );
                        if( lua_isnumber( L, -1 ) ) data[i->second].textureRect.height = static_cast<int>( lua_tonumber( L, -1 ) );
                        else std::cout << "Error loading UnitType " << i->first.c_str() << "TextureRect" << std::endl;
                        lua_pop( L, 1 );
                    }
                    // Commented out because not all units have texRects. Not having one indicates that they use the entire texture.
                    //else std::cout << "Error loading UnitType " << i->first.c_str() << " range" << std::endl;
                    lua_pop( L, 1 );
                    lua_getfield(  L, -1, "Abilities" );
                    if( lua_istable( L ,-1 ) )
                    {
                        lua_pushnil( L );
                        while( lua_next( L, -2 ) != 0 )
                        {
                            if( lua_isstring( L, -1 ) ) data[i->second].abilities.push_back( lua_tostring( L, -1 ) );
                            else std::cout << "Error loading UnitType " << i->first.c_str() << "Ability" << std::endl;
                            lua_pop( L, 1 ); // anon string value
                        }
                        lua_pop( L, 1 );
                    }
                    lua_pop( L, 1 ); // Abilities table
                }
                lua_pop( L, 1 ); // defintion table
            }
            lua_pop( L, 1 ); // anon table
        }else std::cout << "Error reading UnitTypes.lua" << std::endl;
        lua_close( L );
        return data;
}( ); // initializeUnitTypesData
 static std::vector<BuildingData> initializeBuildingTypeData = []( ) -> std::vector<BuildingData> {
    std::vector<BuildingData> data( BuildingTypeMap.size( ) );
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_getglobal( L, "debug" );
    lua_getfield( L, -1, "traceback" );
    lua_replace( L, -2 );
    luaL_loadfile( L, "Game/Lua/Buildings.lua" );
     if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
        luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
        std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
        throw( lua_tostring( L, -1 ) );
     }
     if( lua_istable( L, -1 ) ) // Anon table
     {
        for( auto i = BuildingTypeMap.begin(); i != BuildingTypeMap.end(); ++i )
        {
            lua_getfield( L, -1, i->first.c_str( ) );
            if( lua_istable( L, -1 ) ) // UnitType Definition
            {
                lua_getfield( L, -1, "type" );
                if( lua_isstring( L, -1 ) ) data[i->second].type = lua_tostring( L, -1 );
                else std::cout << "Error loading UnitType " << i->first.c_str() << "type" << std::endl;
                lua_pop( L, 1 );

                lua_getfield( L, -1, "ownedBy" );
                if( lua_isstring( L, -1 ) )
                {
                    std::string temp = lua_tostring( L, -1 );
                    if( temp == "NONE" )  data[i->second].category     = Category::NeutralBuilding;
                    else if( temp == "RED" )  data[i->second].category = Category::RedBuilding;
                    else if( temp == "BLUE" ) data[i->second].category = Category::BlueBuilding;
                    else std::cout << "There was an error reading the Buildings.lua file check ownedBy variables." << std::endl;
                }
                else std::cout << "Error loading UnitType " << i->first.c_str() << "ownerBy" << std::endl;
                lua_pop( L, 1 );

                lua_getfield( L, -1, "goldPerTurn" );
                if( lua_isnumber( L, -1 ) ) data[i->second].goldPerTurn = static_cast<int>( lua_tonumber( L, -1 ) );
                else std::cout << "Error loading UnitType " << i->first.c_str() << "goldPerTurn" << std::endl;
                lua_pop( L, 1 );

                lua_getfield( L, -1, "texture" );
                if( lua_isstring( L, -1 ) ) data[i->second].textureID = lua_tostring( L, -1 );
                else std::cout << "Error loading UnitType " << i->first.c_str() << "texture" << std::endl;
                lua_pop( L, 1 );

                // NEED TO ADD TEXTURE RECTS FOR ANIMATIONS HERE!!!!!!!!
            }
            lua_pop( L, 1 ); // defintion table
        }
        lua_pop( L, 1 ); // anon table
    }
    else std::cout << "Error reading Warps.lua" << std::endl;

    lua_close( L );
    return data;
}( ); // initializeBuildingTypesData
// Left over from western rpg game
static std::vector<ActorData> initializeActorData = []() -> std::vector<ActorData> {
        std::vector<ActorData> data( ActorMap.size( ) );
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);
        lua_getglobal( L, "debug" );
        lua_getfield( L, -1, "traceback" );
        lua_replace( L, -2 );
        luaL_loadfile( L, "Game/Lua/Resources/Actors.lua" );
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
// this is a special table designed to help the grid decide where units can move on any given turn.
static std::vector<std::vector<float>> inititializeUnitMovementCostTable = []() -> std::vector<std::vector<float>> {
    std::vector<std::vector<float>> data;
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_getglobal( L, "debug" );
    lua_getfield( L, -1, "traceback" );
    lua_replace( L, -2 );
    luaL_loadfile( L, "Game/Lua/UnitMovementCost.lua" );
    if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
        luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
        std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
        throw( lua_tostring( L, -1 ) );
    }
    if( lua_istable( L, -1 ) ) // Anon table
    {
        // load data with "2d array" found in lua file
        lua_pushnil( L );
        while( lua_next( L, -2 ) != 0 )
        {
            data.push_back( std::vector<float>( ) );
            // iterate through numbers in array
            lua_pushnil( L );
            while( lua_next( L, -2 ) != 0 )
            {
                if( lua_isnumber( L, -1 ) ) data.back( ).push_back( static_cast<float>( lua_tonumber( L, -1 ) ) );
                else std::cout << "ERROR reading UnitMovementCost table from lua" << std::endl;
                lua_pop( L, 1 ); // clean up iterations
            }
            lua_pop( L, 1 ); // clean up iterations
        }
        lua_pop( L, 1 ); // anon table from the top
    }
    lua_close( L );
    return data;
}( );


#endif // DATATABLES_HPP

