#include "SinglePlayerBattle.hpp"
#include "Core/MusicPlayer.hpp"
#include "Core/Globals.hpp"
#include "Core/Utility.hpp"
#include "Gui/Button.hpp"
#include "Gui/Container.hpp"
/// #include "Game/WifeBot.hpp"
#include <memory>

SinglePlayerBattle::SinglePlayerBattle(States::ID id, StateStack& stack, Context context , unsigned int level)
    : State( id, stack, context )
    , mWorld( &context, &stack, *context.window, *context.fonts, *context.sounds, level, false, false )
    , mPlayer( &mWorld, nullptr, Category::Player, context.keys1 )
    , mLevel( level )
{
}

SinglePlayerBattle::~SinglePlayerBattle( )
{
    /*
    lua_State* L = luaL_newstate( );
    luaL_openlibs( L );

    // call with error checking
    lua_getglobal( L, "debug" );
    lua_getfield( L, -1, "traceback" );
    lua_replace( L, -2 );
    luaL_loadfile( L, "Game/Game.lua" );
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
    } else std::cout << "Error reading Game.lua data table" << std::endl;
    lua_close( L );
    */
    //delete mPlayer2;
}

void SinglePlayerBattle::draw( )
{
    mWorld.draw( );
}

bool SinglePlayerBattle::update( sf::Time dt )
{
    mWorld.update( dt );

    CommandQueue& commands = mWorld.getCommandQueue( );
    try { mPlayer.handleRealtimeInput( commands ); }
    catch( std::exception& e ) {
        std::cout << "There was an exception during the Player1_Input update: " << e.what( ) << std::endl;
    }
    return true;
}

bool SinglePlayerBattle::handleEvent( const sf::Event& event )
{
    // Escape pressed, trigger the pause screen -- 9 is the id of the start button
    if( ( event.type == sf::Event::JoystickButtonReleased && event.joystickButton.button == 9 ) || ( event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape ))
        requestStackPush( States::Pause );
    else if( event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left )
    {
        // create a new event with delta mouse pos
        sf::Event newEvent;
        newEvent.type = sf::Event::MouseButtonReleased;
        newEvent.mouseButton.button = sf::Mouse::Left;
        newEvent.mouseButton.x = event.mouseButton.x - mWorld.mDeltaMousePosition.x;
        newEvent.mouseButton.y = event.mouseButton.y - mWorld.mDeltaMousePosition.y;
        mPlayer.handleEvent( newEvent, mWorld.getCommandQueue( ) );
    }
    else
    {
        mWorld.handleEvent( event );
    }
    return true;
}
