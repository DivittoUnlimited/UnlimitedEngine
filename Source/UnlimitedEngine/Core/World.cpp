 #include "Core/World.hpp"

#include "TextNode.hpp"
#include "DataTables.hpp"
#include "State.hpp"
#include "ParticleNode.hpp"
#include "Core/Player.hpp"
#include "Core/MusicPlayer.hpp"
#include "Core/SoundPlayer.hpp"
#include "Core/Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RectangleShape.hpp> // debugging only erase me!!

#include <algorithm>
#include <cmath>
#include <limits>

static sf::Vector2f SpawnPosition;

World::World( sf::RenderTarget &outputTarget, TextureManager &textures, FontManager& fonts, MusicPlayer& music, SoundPlayer& sounds, State::Context context )
: mTarget( outputTarget )
, mSceneTexture( )
, mWorldView( outputTarget.getDefaultView( ) )
, mTextures( textures )
, mFonts( fonts )
, mMusic( music )
, mSounds( sounds )
, mContext( context )
, mWindowSprite(  )
, mSceneGraph( )
, mSceneLayers( )
{
    if( !mSceneTexture.create( sf::VideoMode::getDesktopMode().width + 100, mTarget.getSize( ).y ) ) std::cout << "Render ERROR" << std::endl;
    mSceneTexture.setView( mWorldView );
    buildScene( );
}

World::~World( )
{

}

void World::update( sf::Time dt )
{

}

void World::draw( )
{
    if( PostEffect::isSupported( ) )
    {
        mSceneTexture.clear( sf::Color( 34, 120, 34 ) );

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

CommandQueue& World::getCommandQueue( )
{
    return mCommandQueue;
}

bool matchesCategories( std::pair<SceneNode*, SceneNode*>& colliders, Category::Type type1, Category::Type type2 )
{
    unsigned int category1 = colliders.first->getCategory( );
    unsigned int category2 = colliders.second->getCategory( );

    // Make sure first pair entry has category type1 and second has type2
    if( type1 & category1 && type2 & category2 )
    {
        return true;
    }
    else if ( type1 & category2 && type2 & category1 )
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
    std::set<std::pair<SceneNode*, SceneNode*>> collisionPairs;
    mSceneGraph.checkSceneCollision( *mSceneLayers[UpperAir], collisionPairs );

    for( std::pair<SceneNode*, SceneNode*> pair : collisionPairs )
    {

    }
}

void World::buildScene( )
{
    // Initialize the different layers
    for( std::size_t i = 0; i < LayerCount; ++i )
    {
        Category::Type category = ( i == LowerAir ) ? Category::SceneAirLayer : Category::None;

        SceneNode::Ptr layer( new SceneNode( category ) );
        mSceneLayers[i] = layer.get( );

        mSceneGraph.attachChild( std::move( layer ) );
    }

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
        lua_getfield( L, -1, "Particles" );
        if( lua_istable( L, -1 ) )
        {
            lua_pushnil( L );
            while( lua_next( L, -2 ) != 0 )
            {
                //std::unique_ptr<ParticleNode> node( new ParticleNode( ParticleMap.at( lua_tostring( L, -1 ) ), mTextures ) );
                //mSceneLayers[LowerAir]->attachChild( std::move( node ) );
                lua_pop( L, 1 );
            }
        }
        lua_pop( L, 1 ); // Particles table

        /*
        lua_getfield( L, -1, "PlayerTexture" );
        if( lua_isstring( L, -1 ) )
        {
            // Add player's aircraft
            std::unique_ptr<Aircraft> player( new Aircraft( AircraftMap.at( lua_tostring( L, -1 ) ), mTextures, &mSounds, mFonts ) );
            mPlayerAircraft = player.get( );
            mPlayerAircraft->setPosition( SpawnPosition );
            mSceneLayers[UpperAir]->attachChild(std::move( player ) );
        }else std::cout << "Error Reading background Texture! World::BuildScene( )" << std::endl;
        lua_pop( L, 1 );
        */
    }
    lua_close( L );

}

