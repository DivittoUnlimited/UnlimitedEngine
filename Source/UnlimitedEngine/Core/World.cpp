#include "Core/World.hpp"

#include "TextNode.hpp"
#include "DataTables.hpp"
#include "State.hpp"
#include "ParticleNode.hpp"
#include "Tiled/TiledManager.hpp"
#include "Core/Player.hpp"
#include "Core/MusicPlayer.hpp"
#include "Core/SoundPlayer.hpp"
#include "Core/Utility.hpp"

#include "Objects/Wall.hpp"
#include "Objects/Actor.hpp"
#include "Objects/Warp.hpp"
#include "Objects/Item.hpp"

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
, mPlayer( nullptr )
{
    mWorldView.zoom( .70 );
    if( !mSceneTexture.create( outputTarget.getView().getSize().x, mTarget.getSize( ).y ) ) std::cout << "Render ERROR" << std::endl;
    mSceneTexture.setView( mWorldView );
    buildScene( );
    //mWorldView.setCenter( mPlayer->getPosition() );
}

World::~World( )
{

}

void World::update( sf::Time dt )
{
    //mWorldView.setCenter( mPlayer->getPosition() );
    while( !mCommandQueue.isEmpty( ) )
        mSceneGraph.onCommand( mCommandQueue.pop( ), dt );
    mSceneGraph.update( dt, mCommandQueue );
    handleCollisions( );
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
        // mBloomEffect.apply( mSceneTexture, mTarget ); // When u re-add bloom effects remove the window sprite scale in the cinstructor for some reason bloom effect messes with that
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
    mSceneGraph.checkSceneCollision( *mSceneLayers[LayerMap.at( "ObjectLayer" )], collisionPairs );

    for( std::pair<SceneNode*, SceneNode*> pair : collisionPairs )
    {
        if( matchesCategories( pair, Category::Player, Category::Wall ) )
        {
              auto& player = static_cast<Actor&>( *pair.first );
              player.setVelocity( 0, 0 );
        }
        else if( matchesCategories( pair, Category::Player, Category::Warp ) )
        {
             auto& player = static_cast<Actor&>( *pair.first );
             auto& warp = static_cast<Warp&>( *pair.second );
             player.setPosition( warp.getNewPosition( ) );
        }
        else if( matchesCategories( pair, Category::Player, Category::NPC ) )
        {
            ///
            /// Activate dialog engine here!!! lol ;p
            ///
            std::cout <<  "Hey Whatch'it man!" << std::endl;
        }
        else {
            std::cout << "Error occured while checking Collision something is being checked that shouldn't be!" << std::endl;
        }
    }
}

void World::buildScene( )
{
    // load TiledMap
    std::cout << "World::buildScene using HARCODED filepath to load Tiled map untill levels can be loaded properly instead of using Game.lua for everything. TiledMaps get loaded from level files" << mContext.tiledMapFilePath << std::endl;
    Tiled::TiledMap map = Tiled::loadFromFile( "Game/Levels/Greenville.lua" ); //mContext.TiledMapFilePath );

    for( unsigned int i = 0; i < map.layers.size(); ++i )
    {
        if( map.layers[i].type == "tilelayer" )
        {
            auto tileSets = map.tileSets[0];
            std::unique_ptr<VertexArrayNode> layer( new VertexArrayNode( ) );
            if( !layer.get()->load( mTextures.get( TextureMap.at( tileSets.name ) ), sf::Vector2u( tileSets.tileWidth, tileSets.tileHeight ), map.layers[i].data , map.width, map.height ) )
                std::cout << "ERROR loading TiledMap! BuildScene ln: 155" << std::endl;
            else {
                mSceneLayers.push_back( layer.get( ) );
                mSceneGraph.attachChild( std::move( layer ) );
            }
        }
        else if( map.layers[i].type == "objectgroup" )
        {
            auto layer = map.layers[i];
            SceneNode::Ptr node( new SceneNode( Category::ObjectLayer ) );
            // Build loop through all objects
            for( unsigned int i = 0; i < layer.objects.size(); ++i )
            {
                 auto object = layer.objects.at( i );
                 if( object.type == "Player" )
                 {
                     // std::unique_ptr<Actor> actor( new Actor( object, TextureMap.at( tiles[object.gid].texID ), tiles[object.gid].rect, mTextures, &mSounds, mFonts ) );
                     // this->mPlayer = actor.get( );
                     // node.get( )->attachChild( std::move( actor ) );
                 }
                 else if( object.type == "Wall" )
                 {
                     std::unique_ptr<Wall> wall( new Wall( object, sf::RectangleShape( sf::Vector2f( object.width, object.height ) ) ) );
                     node.get( )->attachChild( std::move( wall ) );
                 }else if( object.type == "Warp" )
                 {
                     std::unique_ptr<Warp> warp( new Warp( object, sf::RectangleShape( sf::Vector2f( object.width, object.height ) ), mFonts ) );
                     node.get( )->attachChild( std::move( warp ) );
                 }else if( object.type == "Actor" )
                 {
                    // std::unique_ptr<Actor> actor( new Actor( object, TextureMap.at( tiles[object.gid].texID ), tiles[object.gid].rect, mTextures, &mSounds, mFonts ) );
                    // node.get( )->attachChild( std::move( actor ) );
                 }else if( object.type == "Item" )
                 {
                     // std::unique_ptr<Item> item( new Item( ) );
                     // node.get( )->attachChild( std::move( item ) );
                 }else
                     std::cout << "Invalid object being loaded from tile map" << std::endl;
            }
                mSceneLayers.push_back( node.get( ) );
                mSceneGraph.attachChild( std::move( node ) );


        }
        else if( map.layers[i].type == "imageLayer" )
        {
            // Create Layer
            SceneNode::Ptr node( new SceneNode( Category::ImageLayer ) );
            mSceneLayers[i] = node.get( );
            mSceneGraph.attachChild( std::move( node ) );

            // load texture for use by SpriteNode
            sf::Texture& texture = mTextures.get( TextureMap.at( map.tileSets[i].name ) );
            texture.setSmooth( true );

            // Create SpriteNode to hold image attach to layer node
            std::unique_ptr<SpriteNode> backgroundImage( new SpriteNode( texture ) );
            mSceneLayers[i]->attachChild( std::move( backgroundImage ) );
        }
    }



/*
    struct Tile {
        std::string texID;
        sf::Rect<int> rect;
    };

    std::vector<Tile> tiles = std::vector<Tile>( );
    tiles.push_back( Tile() );
    tiles[0].texID = "NONE";
    tiles[0].rect = sf::Rect<int>( 0, 0, 16, 16 ); // HARD VALUES THAT NEED TO BE REMOVED DO NOT REMOVE ME UNTILL ITS DONE!!!!!!!!!!!!!!

    for( unsigned int i = 0; i < map.tileSets.size(); ++i )
    {
        // define loops to divide up image
        unsigned int y = 0;
        unsigned int x = 0;
        unsigned int tileWidth = map.tileSets[i].tileWidth;
        unsigned int tileHeight =  map.tileSets[i].tileHeight;
        std::string name = map.tileSets[i].name;

        while( y < ( map.tileSets[i].imageHeight - tileHeight) )
        {
            while( x < map.tileSets[i].imageWidth )
            {
                // Create tile
                Tile tile;
                tile.texID = name;
                tile.rect = sf::Rect<int>( x, y, tileWidth, tileHeight );
                // add tile to set of possible tiles to use later it's index in vector is it's id as found in testMap.lua
                tiles.push_back( tile );
                x += tileWidth;
            }
            x = 0;
            y += tileHeight;
        }
    }

    for( unsigned int i = 0; i < map.layers.size(); ++i )
    {
        auto layer = map.layers[i];
        if( layer.type == "tilelayer" )
        {
            // Create Layer
            SceneNode::Ptr node( new SceneNode( Category::TileLayer ) );

            // use tiles vector to build layer
            unsigned int x = 0;
            unsigned int y = 0;
            for( unsigned int k = 0; k < layer.data.size(); ++k )
            {
                //std::cout << "Layer.data[" << k << "] == " << layer.data[k] << std::endl;
                if( layer.data[k] != 0 )
                {

                    // Create Sprite Node using appropriate tile and then add it to the layer dont forget to add layer to the graph!!!!!!
                    std::unique_ptr<SpriteNode> newTile( new SpriteNode( mTextures.get( TextureMap.at( tiles[layer.data[k]].texID ) ) ) );
                    // edit tile for placement
                    newTile->getSprite( )->setTextureRect( tiles[layer.data[k]].rect );
                    newTile->setPosition( x, y );
                    node->attachChild( std::move( newTile ) );
                }
                // move down the row to the left unless at the left most side in which case move down to the next row and start at the begining again
                if ( x < ( (layer.width-1) * tiles[layer.data[k]].rect.width) )
                    x += tiles[layer.data[k]].rect.width;
                else
                {
                    x = 0;
                    y += tiles[layer.data[k]].rect.height;
                }
            }
            mSceneLayers.push_back( node.get( ) );
            mSceneGraph.attachChild( std::move( node ) );
        }

        else if( layer.type == "objectgroup" )
        {
            SceneNode::Ptr node( new SceneNode( Category::ObjectLayer ) );
            // Build loop through all objects
            for( unsigned int i = 0; i < layer.objects.size(); ++i )
            {
                auto object = layer.objects.at( i );
                if( object.type == "Player" )
                {
                    std::unique_ptr<Actor> actor( new Actor( object, TextureMap.at( tiles[object.gid].texID ), tiles[object.gid].rect, mTextures, &mSounds, mFonts ) );
                    this->mPlayer = actor.get( );
                    node.get( )->attachChild( std::move( actor ) );
                }
                else if( object.type == "Wall" )
                {
                    std::unique_ptr<Wall> wall( new Wall( object, sf::RectangleShape( sf::Vector2f( object.width, object.height ) ) ) );
                    node.get( )->attachChild( std::move( wall ) );
                }else if( object.type == "Warp" )
                {
                    std::unique_ptr<Warp> warp( new Warp( object, sf::RectangleShape( sf::Vector2f( object.width, object.height ) ), mFonts ) );
                    node.get( )->attachChild( std::move( warp ) );
                }else if( object.type == "Actor" )
                {
                    std::unique_ptr<Actor> actor( new Actor( object, TextureMap.at( tiles[object.gid].texID ), tiles[object.gid].rect, mTextures, &mSounds, mFonts ) );
                    node.get( )->attachChild( std::move( actor ) );
                }else if( object.type == "Item" )
                {
                    // std::unique_ptr<Item> item( new Item( ) );
                    // node.get( )->attachChild( std::move( item ) );
                }else
                    std::cout << "Invalid object being loaded from tile map" << std::endl;
             }
            mSceneLayers.push_back( node.get( ) );
            mSceneGraph.attachChild( std::move( node ) );
        }
        else if( layer.type == "imagelayer" )
        {
            // Create Layer
            SceneNode::Ptr node( new SceneNode( Category::ImageLayer ) );
            mSceneLayers[i] = node.get( );
            mSceneGraph.attachChild( std::move( node ) );

            // load texture for use by SpriteNode
            sf::Texture& texture = mTextures.get( TextureMap.at( layer.name ) );
            texture.setSmooth( true );

            // Create SpriteNode to hold image attach to layer node
            std::unique_ptr<SpriteNode> backgroundImage( new SpriteNode( texture ) );
            mSceneLayers[i]->attachChild( std::move( backgroundImage ) );
        }
    }
    */
    ///
    ///
    /// Where does the level init happen??
    ///     - refering to when there are more then one level and not just a single "world"
    ///     - See flow chart on your phone.


    /*
    /// HEY FUCK FACE!!!!!! READ ME!!!!!
    ///
    ///
    ///
    /// Why is this done here and not with the rest of the assets in the load state?
    /// This needs to be handled way better!!!!
    ///
    ///
    ///
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
    }
    lua_close( L );
    */
}

