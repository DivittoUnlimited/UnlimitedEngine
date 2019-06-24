#include "GameState.hpp"
#include "Core/MusicPlayer.hpp"
#include "Core/Globals.hpp"

GameState::GameState( States::ID id, StateStack& stack, Context context )
    : State( id, stack, context )
    , mWorld( *context.window, *context.fonts, *context.sounds, false, false )
    , mPlayer( nullptr, Category::Player, context.keys1 )
    //, mPlayer2( new Player( nullptr, Category::Player2, context.keys2 ) )
    //, mBlue2( Category::Blue2 )
    , mRed1( Category::Red1 )
    //, mRed2( Category::Red2 )
{

}

GameState::~GameState( )
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

void GameState::draw( )
{
    mWorld.draw( );
}

bool GameState::update( sf::Time dt )
{
    mWorld.update( dt );

    /* Old Example of "Game" logic update with flag scores to show victory...
    if (!mWorld.hasAlivePlayer())
    {
        mPlayer.setMissionStatus(Player::MissionFailure);
        requestStackPush(States::GameOver);
    }
    else if (mWorld.hasPlayerReachedEnd())
    {
        mPlayer.setMissionStatus(Player::MissionSuccess);
        requestStackPush(States::MissionSuccess);
    }
    */

    CommandQueue& commands = mWorld.getCommandQueue( );
    try { mPlayer.handleRealtimeInput( commands ); }
    catch( std::exception& e ) {
        std::cout << "There was an exception during the Player1_Input update: " << e.what( ) << std::endl;
    }

    /*
    if( mPlayer2 != nullptr )
        try { mPlayer2->handleRealtimeInput( commands ); }
        catch( std::exception& e ) {
            std::cout << "There was an exception during the Player2_Input update: " << e.what( ) << std::endl;
        }
*/
   // mBlue2.updateCurrent( dt, commands );

    mRed1.updateCurrent( dt, commands );
  //  mRed2.updateCurrent( dt, commands );

    return true;
}

bool GameState::handleEvent( const sf::Event& event )
{
    mPlayer.handleEvent( event, mWorld.getCommandQueue( ) );

    // Escape pressed, trigger the pause screen -- 9 is the id of the start button
    if( ( event.type == sf::Event::JoystickButtonReleased && event.joystickButton.button == 9 ) || ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape )) {
        requestStackPush( States::Pause );
    }

	return true;
}
