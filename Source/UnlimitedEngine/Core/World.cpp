#include "World.hpp"

#include "Objects/Arena.hpp"

enum Layers {
    ObjectLayer = 0
};

Arena* ARENA = new Arena( );

World::World( sf::RenderTarget& outputTarget, FontManager& fonts, SoundPlayer& sounds, bool networked, bool isLocalMultiplayer )
    : mTarget( outputTarget )
    , mSceneTexture( )
    , mWorldView( mTarget.getDefaultView( ) )
    , mTextures(  )
    , mFonts( fonts )
    , mSounds( sounds )
    , mWindowSprite(  )
    , mSceneGraph( )
    , mSceneLayers( )
    , mNetworkedWorld( networked )
    , mLocalMultiplayerWorld( isLocalMultiplayer )
    , mNetworkNode( nullptr )
    , mTeamAScore( 3 )
    , mTeamBScore( 3 )
{
    if( !mSceneTexture.create( mTarget.getView().getSize().x, mTarget.getSize( ).y ) ) std::cout << "Render ERROR" << std::endl;
    mSceneTexture.setView( mWorldView );

    buildScene( );
}

World::~World( void )
{
}

void World::draw( )
{
    if( PostEffect::isSupported( ) )
    {
        mSceneTexture.clear( sf::Color( 0, 0, 0 ) );

        mSceneTexture.setView( mWorldView );
        mSceneTexture.draw( mSceneGraph );
        mSceneTexture.display( );

        mWindowSprite.setTexture( mSceneTexture.getTexture( ) );

        mTarget.draw( sf::Sprite( mWindowSprite ) );
        mBloomEffect.apply( mSceneTexture, mTarget );
    }
    else
    {
        mTarget.setView( mWorldView );
        mTarget.draw( mSceneGraph );
    }
}

bool World::update( sf::Time dt )
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
    mSceneGraph.removeWrecks( );
    return true;
}


CommandQueue& World::getCommandQueue( )
{
    return mCommandQueue;
}

StarShip* World::getStarShip( int identifier ) const
{
    if( ARENA->BLUETEAM->starShips.find( identifier ) != ARENA->BLUETEAM->starShips.end( ) )
        return ARENA->BLUETEAM->starShips.at( identifier );
    else if( ARENA->REDTEAM->starShips.find( identifier ) != ARENA->REDTEAM->starShips.end( ) )
        return ARENA->REDTEAM->starShips.at( identifier );
    return nullptr;
}


void World::removeStarShip( int identifier )
{
    StarShip* s = getStarShip( identifier );
    if( s )
    {
        s->destroy( );
        //mPlayerStarShips.erase( std::find( mPlayerStarShips.begin( ), mPlayerStarShips.end( ), s ) );
    }
}

bool World::pollGameAction( GameActions::Action& out )
{
    return mNetworkNode->pollGameAction( out );
}

bool World::matchesCategories( std::pair<SceneNode*, SceneNode*>& colliders, Category::Type type1, Category::Type type2 )
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

void World::handleCollisions( )
{
    CollisionMan::QUAD_TREE.setRootRect( 0, 0, 1024, 1024 ); // mod this to move with player/Camera!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    std::set<std::pair<SceneNode*, SceneNode*>> collisionPairs = CollisionMan::update( mSceneLayers[0] );

    for( std::pair<SceneNode*, SceneNode*> pair : collisionPairs )
    {
        if( matchesCategories( pair, Category::BlueTeam, Category::FlagRed ) || matchesCategories( pair, Category::Player, Category::FlagRed ) )
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
        else if( matchesCategories( pair, Category::RedTeam, Category::FlagBlue ) || matchesCategories( pair, Category::Player2, Category::FlagBlue ) )
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
        else if( matchesCategories( pair, Category::FlagBlue, Category::GoalRed ) )
        {
            auto& flag = static_cast<Flag&>( *pair.first );

            StarShip* starShip = flag.getStarShip();
            starShip->setHasFlag( false );
            mSceneLayers[0]->attachChild( starShip->detachChild( flag ) ); // removes flag from player reattaches to layer
            flag.setPosition( flag.getStartingPos( ) );
            ARENA->BLUETEAM->goal->scale( .5, 1 );
            ARENA->REDTEAM->goal->scale( 1.5, 1 );
            if( mTeamAScore > 0 )
            {
                mTeamAScore--;
                mTeamBScore++;
                std::cout << "Current Score: Red: " << mTeamBScore << " Blue: " << mTeamAScore << std::endl;
            }
            else {
                std::cout << "Red Wins!" << std::endl;
            }
        }
        else if( matchesCategories( pair, Category::FlagRed, Category::GoalBlue ) )
        {
            auto& flag = static_cast<Flag&>( *pair.first );

            StarShip* starShip = flag.getStarShip();
            starShip->setHasFlag( false );
            mSceneLayers[0]->attachChild( starShip->detachChild( flag ) ); // removes flag from player reattaches to layer
            flag.setPosition( flag.getStartingPos( ) );
            ARENA->REDTEAM->goal->scale( .5, 1 );
            ARENA->BLUETEAM->goal->scale( 1.5, 1 );
            if( mTeamBScore > 0 )
            {
                mTeamBScore--;
                mTeamAScore++;
                std::cout << "Current Score: Red: " << mTeamBScore << " Blue: " << mTeamAScore << std::endl;
            }
            else {
                std::cout << "Blue Wins!" << std::endl;
            }
        }
    }
}

void World::buildScene( void )
{
    // Create layers...

    // Object layer
    std::unique_ptr<SceneNode> objectLayer( new SceneNode( Category::ObjectLayer ) );
    mSceneLayers.push_back( objectLayer.get( ) );
    mSceneGraph.attachChild( std::move( objectLayer ) );

    // Create objects in the world
    // Goals

    std::unique_ptr<Goal> teamAGoal( new Goal( Category::GoalBlue ) );
    ARENA->BLUETEAM->goal = teamAGoal.get( );
    mSceneLayers[0]->attachChild( std::move( teamAGoal ) );
    std::unique_ptr<Goal> teamBGoal( new Goal( Category::GoalRed ) );
    ARENA->REDTEAM->goal = teamBGoal.get( );
    mSceneLayers[0]->attachChild( std::move( teamBGoal ) );

    // Bumpers
    std::unique_ptr<Bumper> bumperBlue1( new Bumper( Category::BumperBlue ) );
    bumperBlue1->setPosition( WINDOW_WIDTH / 2, WINDOW_HEIGHT - 170 );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( bumperBlue1 ) );

    std::unique_ptr<Bumper> bumperRed1( new Bumper( Category::BumperRed ) );
    bumperRed1->setPosition( WINDOW_WIDTH / 2, 170 );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( bumperRed1 ) );

    // Blue StarShips
    std::unique_ptr<StarShip> blue1( new StarShip( Category::Player ) );
    blue1->setPosition( WINDOW_WIDTH / 2, WINDOW_HEIGHT - 200 );
    blue1->setIdentifier( Category::Player );
    ARENA->BLUETEAM->starShips[Category::Blue1] = blue1.get( );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( blue1 ) );

    std::unique_ptr<StarShip> blue2( new StarShip( Category::Player ) );
    blue2->setPosition( 250, WINDOW_HEIGHT - 150 );
    ARENA->BLUETEAM->starShips[Category::Blue2] = blue2.get( );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( blue2 ) );

    // Red StarShips
    if( mLocalMultiplayerWorld )
    {
        std::unique_ptr<StarShip> Red1( new StarShip( Category::Player2 ) );
        Red1->setPosition( WINDOW_WIDTH / 2, 200 );
        Red1->rotate( 180.0f );
        Red1->setIdentifier( Category::Player2 );
        ARENA->REDTEAM->starShips[Category::Red1] = Red1.get( );
        mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( Red1 ) );
    }
    else {
        std::unique_ptr<StarShip> Red1( new StarShip( Category::Red1 ) );
        Red1->setPosition( WINDOW_WIDTH / 2, 200 );
        Red1->rotate( 180.0f );
        Red1->setIdentifier( Category::Red1 );
        ARENA->REDTEAM->starShips[Category::Red1] = Red1.get( );
        mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( Red1 ) );
    }

    std::unique_ptr<StarShip> red2( new StarShip( Category::Red1 ) );
    red2->setPosition( 250, 150 );
    red2->rotate( 180.0f );
    ARENA->REDTEAM->starShips[Category::Red2] = red2.get( );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( red2 ) );

    // Blue Teams Starting flags
    std::unique_ptr<Flag> flag3( new Flag( Category::FlagBlue, sf::Vector2f( 174, 678 ) ) );
    ARENA->BLUETEAM->flags.push_back( flag3.get( ) );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( flag3 ) );
    std::unique_ptr<Flag> flag4( new Flag( Category::FlagBlue, sf::Vector2f( WINDOW_WIDTH / 2, 678 ) ) );
    ARENA->BLUETEAM->flags.push_back( flag4.get( ) );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( flag4 ) );
    std::unique_ptr<Flag> flag6( new Flag( Category::FlagBlue, sf::Vector2f( 850, 678 ) ) );
    ARENA->BLUETEAM->flags.push_back( flag6.get( ) );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( flag6 ) );

    // Red Teams starting flags
    std::unique_ptr<Flag> flag1( new Flag( Category::FlagRed, sf::Vector2f( 174, 90 ) ) );
    ARENA->REDTEAM->flags.push_back( flag1.get( ) );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( flag1 ) );
    std::unique_ptr<Flag> flag2( new Flag( Category::FlagRed, sf::Vector2f( WINDOW_WIDTH / 2, 90 ) ) );
    ARENA->REDTEAM->flags.push_back( flag2.get( ) );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( flag2 ) );
    std::unique_ptr<Flag> flag5( new Flag( Category::FlagRed, sf::Vector2f( 850, 90 ) ) );
    ARENA->REDTEAM->flags.push_back( flag5.get( ) );
    mSceneLayers.at( Layers::ObjectLayer )->attachChild( std::move( flag5 ) );

    // Players

    // Add network node, if necessary
    if( mNetworkedWorld )
    {
        std::unique_ptr<NetworkNode> networkNode( new NetworkNode( ) );
        mNetworkNode = networkNode.get( );
        mSceneGraph.attachChild( std::move( networkNode ) );
        std::cout << "BuildScene Complete on a networked world!!" << std::endl;
    }

}
