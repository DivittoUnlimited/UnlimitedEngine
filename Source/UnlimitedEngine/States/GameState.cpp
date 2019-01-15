#include "GameState.hpp"
#include "Core/MusicPlayer.hpp"
#include "Core/Globals.hpp"

GameState::GameState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mWorld( *context.window, *context.textures, *context.fonts, *context.music, *context.sounds, context )
, mPlayer( *context.player )
{
}

GameState::~GameState( )
{
    lua_State* L = luaL_newstate( );
    luaL_openlibs( L );

    // call with error checking
    lua_getglobal( L, "debug" );
    lua_getfield( L, -1, "traceback" );
    lua_replace( L, -2 );
    luaL_loadfile( L, CurrentLuaFile.c_str() );
    if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
        luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
        std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
    }
    if( lua_istable( L, -1 ) )
    {
        lua_pushstring( L, "Textures" );
        lua_gettable( L, -2 );
        if( lua_istable( L, -1 )  )
        {
            lua_pushnil( L );
            while( lua_next( L, -2 ) != 0 )
            {
                mContext.textures->remove( TextureMap.at( lua_tostring( L, -1 ) ) );
                lua_pop( L, 1 );
            }
            lua_pop( L ,1 );
        }
        if( lua_istable( L, -1 ) )
        {
            lua_pushstring( L, "Fonts" );
            lua_gettable( L, -2 );
            if( lua_istable( L, -1 )  )
            {
                lua_pushnil( L );
                while( lua_next( L, -2 ) != 0 )
                {
                    mContext.fonts->remove( TextureMap.at( lua_tostring( L, -1 ) ) );
                    lua_pop( L, 1 );
                }
                lua_pop( L ,1 );
            }
        }
        if( lua_istable( L, -1 ) )
        {
            lua_pushstring( L, "SoundEffects" );
            lua_gettable( L, -2 );
            if( lua_istable( L, -1 )  )
            {
                lua_pushnil( L );
                while( lua_next( L, -2 ) != 0 )
                {
                    //mContext.sounds->mSoundBuffers.remove( SoundEffectMap.at( lua_tostring( L, -1 ) ) );
                    lua_pop( L, 1 );
                }
                lua_pop( L ,1 );
            }
        }
    } else std::cout << "Error reading " << CurrentLuaFile.c_str( ) << " data table" << std::endl;
    lua_close( L );
}

void GameState::draw( )
{
    mWorld.draw( );
}

bool GameState::update( sf::Time dt )
{
    mWorld.update( dt );

    CommandQueue& commands = mWorld.getCommandQueue( );
    mPlayer.handleRealtimeInput( commands );

    return true;
}

bool GameState::handleEvent( const sf::Event& event )
{
	// Game input handling
    CommandQueue& commands = mWorld.getCommandQueue( );
    mPlayer.handleEvent( event, commands );

    // Escape pressed, trigger the pause screen -- 9 is the id of the start button
    if( ( event.type == sf::Event::JoystickButtonReleased && event.joystickButton.button == 9 ) || ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape )) {
        requestStackPush( States::Pause );
    }

	return true;
}
