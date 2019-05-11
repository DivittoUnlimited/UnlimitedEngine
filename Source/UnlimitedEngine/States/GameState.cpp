#include "GameState.hpp"
#include "Core/MusicPlayer.hpp"
#include "Core/Globals.hpp"

enum Layers {
    ObjectLayer = 0
};

GameState::GameState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mTarget( *context.window )
, mSceneTexture( )
, mWorldView( mTarget.getDefaultView( ) )
, mTextures( *context.textures )
, mFonts( *context.fonts )
, mMusic( *context.music )
, mSounds( *context.sounds )
, mContext( context )
, mWindowSprite(  )
, mSceneGraph( )
, mSceneLayers( )
, mPlayer( *context.player )
, mTeamAScore( 3 )
, mTeamBScore( 3 )
{
    if( !mSceneTexture.create( mTarget.getView().getSize().x, mTarget.getSize( ).y ) ) std::cout << "Render ERROR" << std::endl;
    mSceneTexture.setView( mWorldView );
    for( unsigned int i = 0; i < StarShips::Count; ++i )
        mStarShips.push_back( new StarShip( ) );
    buildScene( );
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
}

void GameState::draw( )
{
    if( PostEffect::isSupported( ) )
    {
        mSceneTexture.clear( sf::Color( 0, 0, 0 ) );

        mSceneTexture.setView( mWorldView );
        mSceneTexture.draw( mSceneGraph );
        mSceneTexture.display( );

        mWindowSprite.setTexture( mSceneTexture.getTexture( ) );

        mTarget.draw( sf::Sprite( mWindowSprite ) );
        // mBloomEffect.apply( mSceneTexture, mTarget );
    }
    else
    {
        mTarget.setView( mWorldView );
        mTarget.draw( mSceneGraph );
    }
}

bool GameState::update( sf::Time dt )
{
    while( !mCommandQueue.isEmpty( ) )
        mSceneGraph.onCommand( mCommandQueue.pop( ), dt );

    try{ mSceneGraph.update( dt, mCommandQueue ); }
    catch( std::exception& e ) {
        std::cout << "There was an exception in the SceneGraph update: " << e.what( ) << std::endl;
    }

    try{ handleCollisions( ); }
    catch( std::exception& e ) {
        std::cout << "There was an exception during the collision update: " << e.what( ) << "\nDo all your map layer names in lua match from tiled?" << std::endl;
    }
    try { mPlayer.handleRealtimeInput( mCommandQueue ); }
    catch( std::exception& e ) {
        std::cout << "There was an exception during the PlayerInput update: " << e.what( ) << std::endl;
    }

    mSceneGraph.removeWrecks();
    return true;
}

bool GameState::handleEvent( const sf::Event& event )
{
    mPlayer.handleEvent( event, mCommandQueue );

    // Escape pressed, trigger the pause screen -- 9 is the id of the start button
    if( ( event.type == sf::Event::JoystickButtonReleased && event.joystickButton.button == 9 ) || ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape )) {
        requestStackPush( States::Pause );
    }

	return true;
}

CommandQueue& GameState::getCommandQueue( )
{
    return mCommandQueue;
}

bool GameState::matchesCategories( std::pair<SceneNode*, SceneNode*>& colliders, Category::Type type1, Category::Type type2 )
{
    unsigned int category1 = colliders.first->getCategory( );
    unsigned int category2 = colliders.second->getCategory( );

    // Make sure first pair entry has category type1 and second has type2
    if( type1 & category1 && type2 & category2 )
    {
        return true;
    }
    else if( type1 & category2 && type2 & category1 )
    {
        std::swap( colliders.first, colliders.second );
        return true;
    }
    else
    {
        return false;
    }
}

void GameState::handleCollisions( )
{
    CollisionMan::QUAD_TREE.setRootRect( 0, 0, 1024, 1024 ); // mod this to move with player/Camera!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    std::set<std::pair<SceneNode*, SceneNode*>> collisionPairs = CollisionMan::update( mSceneLayers[0] );

    for( std::pair<SceneNode*, SceneNode*> pair : collisionPairs )
    {
        /// ATTENTIION!!! CAN NOT LEAVE PLAYER HERE NEEDS TO BE STARSHIP_A
        if( matchesCategories( pair, Category::Player, Category::FlagB ) || matchesCategories( pair, Category::StarShipB, Category::FlagA ) )
        {
              auto& player = static_cast<StarShip&>( *pair.first );
              if( !player.getHasFlag( ) )
              {
                    auto& flag = static_cast<Flag&>( *pair.second );
                    player.attachChild( mSceneLayers[0]->detachChild( flag ) );
                    flag.setStarShip( &player );
                    flag.setPosition(  0, 45 );
                    player.setHasFlag( true );
              }
        }
        else if( matchesCategories( pair, Category::FlagA, Category::GoalB ) )
        {
            auto& flag = static_cast<Flag&>( *pair.first );

            StarShip* starShip = flag.getStarShip();
            starShip->setHasFlag( false );
            mSceneLayers[0]->attachChild( starShip->detachChild( flag ) ); // removes flag from player reattaches to layer
            flag.setPosition( flag.getStartingPos( ) );
            mGoalA->scale( .5, 1 );
            mGoalB->scale( 1.5, 1 );
            if( mTeamAScore > 0 ) mTeamAScore--;
            else {
                std::cout << "Team B Wins!" << std::endl;
            }
        }
        else if( matchesCategories( pair, Category::FlagB, Category::GoalA ) )
        {
            auto& flag = static_cast<Flag&>( *pair.first );

            StarShip* starShip = flag.getStarShip();
            starShip->setHasFlag( false );
            mSceneLayers[0]->attachChild( starShip->detachChild( flag ) ); // removes flag from player reattaches to layer
            flag.setPosition( flag.getStartingPos( ) );
            mGoalB->scale( .5, 1 );
            mGoalA->scale( 1.5, 1 );
            if( mTeamBScore > 0 ) mTeamBScore--;
            else {
                std::cout << "Team A Wins!" << std::endl;
            }
        }

    }
}

void GameState::buildScene( )
{
    // Create layers...

    // Object layer
    std::unique_ptr<SceneNode> objectLayer( new SceneNode( Category::ObjectLayer ) );
    mSceneLayers.push_back( objectLayer.get( ) );
    mSceneGraph.attachChild( std::move( objectLayer ) );


    // Create objects in the world
    // Goals
    std::unique_ptr<Goal> teamAGoal( new Goal( Owner::TeamA ) );
    mGoalA = teamAGoal.get( );
    mSceneLayers[0]->attachChild( std::move( teamAGoal ) );
    std::unique_ptr<Goal> teamBGoal( new Goal( Owner::TeamB ) );
    mGoalB = teamBGoal.get( );
    mSceneLayers[0]->attachChild( std::move( teamBGoal ) );

    // player 1
    std::unique_ptr<StarShip> blue( new StarShip( true ) );
    mStarShips.at( StarShips::TeamAShip1 ) = blue.get();
    mStarShips.at( StarShips::TeamAShip1 )->setPosition( WINDOW_WIDTH / 2, 500 );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( blue ) );

    //(Player 2)
    std::unique_ptr<StarShip> red( new StarShip() );
    mStarShips.at( StarShips::TeamAShip1 ) = red.get();
    mStarShips.at( StarShips::TeamAShip1 )->setPosition( WINDOW_WIDTH / 2, 200 );
    mStarShips.at( StarShips::TeamAShip1 )->setRotation( 180 );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( red ) );

    // Player 1's Starting flags
    std::unique_ptr<Flag> flag3( new Flag( true, sf::Vector2f( 150, 708 ) ) );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( flag3 ) );
    std::unique_ptr<Flag> flag4( new Flag( true, sf::Vector2f( WINDOW_WIDTH / 2, 708 ) ) );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( flag4 ) );
    std::unique_ptr<Flag> flag6( new Flag( true, sf::Vector2f( 874, 708 ) ) );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( flag6 ) );

    // Player 2's starting flags
    std::unique_ptr<Flag> flag1( new Flag( false, sf::Vector2f( 150, 60 ) ) );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( flag1 ) );
    std::unique_ptr<Flag> flag2( new Flag( false, sf::Vector2f( WINDOW_WIDTH / 2, 60 ) ) );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( flag2 ) );
    std::unique_ptr<Flag> flag5( new Flag( false, sf::Vector2f( 874, 60 ) ) );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( flag5 ) );


    // Bumpers

    // AI Bots

}

