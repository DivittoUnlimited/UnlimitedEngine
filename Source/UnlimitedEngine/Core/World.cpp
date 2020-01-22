#include "World.hpp"
#include "Graphics/VertexArrayNode.hpp"
#include "Tiled/TiledManager.hpp"

enum Layers {
    ObjectLayer = 0
};

namespace
{
    const std::vector<UnitTypeData> UnitDataTable = initializeUnitTypeData;  // initializeUnitTypeData in DataTable.hpp
}

World::World( sf::RenderTarget& outputTarget, FontManager& fonts, SoundPlayer& sounds, unsigned int level, bool networked, bool isLocalMultiplayer )
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
    , mLevel( level )
{
    if( !mSceneTexture.create( static_cast<unsigned int>( mTarget.getView( ).getSize( ).x ), mTarget.getSize( ).y ) ) std::cout << "Render ERROR" << std::endl;
    mSceneTexture.setView( mWorldView );

    buildScene( MediaFileMap.at( "Maps" ).at( mLevel ) );

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

        //mSceneTexture.display( );

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

    // update graphical view of grid based on data view
    if( mMovementGrid.mHasUpdated )
    {
        for( unsigned int i = 0; i < mDrawableGrid.size(); ++i )
        {
            for( unsigned int j = 0; j < mDrawableGrid[i].size(); ++j )
            {
                if( mMovementGrid.mData[j][i].isPossibleAttackPosition )
                {
                    mDrawableGrid[j][i]->getSprite( )->setFillColor( sf::Color::Red );
                    mDrawableGrid[j][i]->getSprite( )->setOutlineColor( sf::Color::Black );
                }
                else if( mMovementGrid.mData[j][i].isPossibleNewLocation )
                {
                    mDrawableGrid[j][i]->getSprite( )->setFillColor( sf::Color::Cyan );
                    mDrawableGrid[j][i]->getSprite( )->setOutlineColor( sf::Color::Black );

                }
                else
                {
                    mDrawableGrid[j][i]->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0  ) );
                    mDrawableGrid[j][i]->getSprite()->setOutlineColor( sf::Color( 0, 0, 0, 0  ) );
                }
            }
        }
        mMovementGrid.mHasUpdated = false;
    }

/*
    try{ handleCollisions( ); }
    catch( std::exception& e ) {
        std::cout << "There was an exception during the collision update: " << e.what( ) << "\nDo all your map layer names in lua match from tiled?" << std::endl;
    }
*/
    mSceneGraph.removeWrecks( );
    return true;
}


CommandQueue& World::getCommandQueue( )
{
    return mCommandQueue;
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

void World::handleEvent( const sf::Event& event )
{
    if( event.type == sf::Event::MouseButtonReleased )
    {
        bool flag = false;
        // Find Which Square on the grid the user clicked
        for( unsigned int i = 0; i < mDrawableGrid.size(); ++i )
        {
            for( unsigned int j = 0; j < mDrawableGrid[i].size(); ++j )
            {
                if( mDrawableGrid[i][j]->getSprite( )->getGlobalBounds( ).contains( sf::Mouse::getPosition(*mWindow).x, sf::Mouse::getPosition(*mWindow).y ) )
                {
                    // Once you have found the square decide what to do based on the state of the square and the "game state"
                    if( mMovementGrid.mData[j][i].isOccupied && !mUnitAwaitingOrders )
                    {
                        std::vector<sf::Vector2i> possibleLocations = mMovementGrid.getPossiblePositions(
                                                                        sf::Vector2i( static_cast<int>( j ), static_cast<int>( i ) ),
                                                                        mCurrentUnits[mMovementGrid.mData[j][i].unitID]->mUnitType,
                                                                        static_cast<unsigned int>( mCurrentUnits[mMovementGrid.mData[j][i].unitID]->mDexterity ) );
                        mSelectedGridIndex = sf::Vector2i( j, i );
                        mSelectedUnitID = mMovementGrid.mData[j][i].unitID;
                        mUnitAwaitingOrders = true;
                        for( unsigned int g = 0; g < possibleLocations.size(); ++g )
                        {
                            mMovementGrid.mData[static_cast<unsigned int>( possibleLocations[g].y )][static_cast<unsigned int>( possibleLocations[g].x )].isPossibleNewLocation = true;
                        }
                        mMovementGrid.mHasUpdated = true;
                    }
                    else if( mMovementGrid.mData[j][i].isOccupied && mUnitAwaitingOrders )
                    {
                        // Attack???
                    }
                    else if( mUnitAwaitingOrders && mMovementGrid.mData[i][j].isPossibleNewLocation )
                    {
                        mCurrentUnits[mSelectedUnitID]->setPosition( j * 32, i * 32 );
                        mMovementGrid.moveUnit( mSelectedGridIndex, sf::Vector2i( static_cast<int>( i ), static_cast<int>( j ) ) );
                        mMovementGrid.clearGrid( );
                        mSelectedUnitID = -1;
                        mUnitAwaitingOrders = false;
                    }
                    else
                    {
                        mMovementGrid.clearGrid( );
                        mUnitAwaitingOrders = false;
                        mSelectedGridIndex = sf::Vector2i( 0, 0 );
                        mSelectedUnitID = -1;
                    }
                    flag = true;
                    break;
                }
            }
            if( flag ) break;
        }
    }

}

void World::handleCollisions( )
{

}

void World::buildScene( std::string tileMapFilePath )
{
    /*
     * NON tiled levels
    // Create layers...

    // Object layer
    std::unique_ptr<SceneNode> objectLayer( new SceneNode( Category::ObjectLayer ) );
    mSceneLayers.push_back( objectLayer.get( ) );
    mSceneGraph.attachChild( std::move( objectLayer ) );

    // Create objects in the world

    // Players


    */

    // load TiledMap
        Tiled::TiledMap map = Tiled::loadFromFile( tileMapFilePath ); //mContext.TiledMapFilePath );

        struct Tile {
            std::string texID;
            sf::Rect<int> rect;
        };

        std::vector<Tile> tiles = std::vector<Tile>( );

        // Add in the first tile as an empty space because the gId in tiled starts at 1
        tiles.push_back( Tile() );
        tiles[0].texID = "NONE";
        tiles[0].rect = sf::Rect<int>( 0, 0, 32, 32 ); // HARD VALUES THAT NEED TO BE REMOVED DO NOT REMOVE ME UNTILL ITS DONE!!!!!!!!!!!!!!
                                                            // Should be able to get these values from tiled file anyway....

        for( unsigned int i = 0; i < map.tileSets.size(); ++i )
        {
            // define loops to divide up image
            int y = 0;
            int x = 0;
            int tileWidth = static_cast<int>( map.tileSets[i].tileWidth );
            int tileHeight =  static_cast<int>( map.tileSets[i].tileHeight );
            std::string name = map.tileSets[i].name;

            // load image into memory
            mTextures.load( TextureMap.at( name ), MediaFileMap.at( "Textures" ).at( TextureMap.at( name ) ) );

            // if your map GID is not lining up you might need to manipulate how many times these loops run to get the number of tiles to run properly.
            while( y < static_cast<int>( map.tileSets[i].imageHeight ) )
            {
                while( x < static_cast<int>( map.tileSets[i].imageWidth ) )
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
            if( map.layers[i].type == "tilelayer" )
            {
                ///
                /// \brief tileSets
                /// DO NOT REMOVE ME UNTILL IT HAS BEEN DONE!!!!
                /// This is a really lazy way to avoid having to deal with multiple image sources to build maps. It doen't work and it needs to be removed!!!!!!
                /// there needs to be a universal way to use as many images as needed to build maps.
                auto tileSets = map.tileSets[0];
                std::cout << "Currently loading tileLayer from tileSet: " << map.tileSets[0].name << "This is hardcoded and needs to be fixed!" << std::endl;

                /*
                 * This is for adding particle effects later
                 *
                 *
                //std::cout << "Particle Effects are being attached in a hardcoded way, NEEDS TO BE IMPROVED!" << std::endl;
                if( map.layers[i].name == "TileLayer3" ) // this is here becuase i needed to attach the particleNode to something that would be rendered above the objects.
                {
                    std::unique_ptr<VertexArrayNode> layer( new VertexArrayNode( Category::ParticleLayer ) );
                    if( !layer.get()->load( mTextures.get( TextureMap.at( tileSets.name ) ), sf::Vector2u( tileSets.tileWidth, tileSets.tileHeight ), map.layers[i].data , map.width, map.height ) )
                        std::cout << "ERROR loading TiledMap! BuildScene ln: 187" << std::endl;
                    else {
                        mSceneLayers.push_back( layer.get( ) );
                        mSceneGraph.attachChild( std::move( layer ) );
                    }
                }
                else
                */
                {
                    std::unique_ptr<VertexArrayNode> layer( new VertexArrayNode(  ) );
                    if( !layer.get()->load( mTextures.get( TextureMap.at( map.tileSets[0].name ) ), sf::Vector2u( tileSets.tileWidth, tileSets.tileHeight ), map.layers[i].data, map.width, map.height ) )
                        std::cout << "ERROR loading TiledMap! BuildScene ln: 249" << std::endl;
                    else {
                        mSceneLayers.push_back( layer.get( ) );
                        mSceneGraph.attachChild( std::move( layer ) );
                        mMovementGrid.buildGrid( map.tileSets[0], map.layers[i] );

                        // build drawable grid
                        for( unsigned int p = 0; p < map.layers[i].height; ++p )
                        {
                            this->mDrawableGrid.push_back( std::vector<RectangleShapeNode*>() );
                            for( unsigned int k = 0; k < map.layers[i].width; ++k )
                            {
                                std::unique_ptr<RectangleShapeNode> rect(
                                            new RectangleShapeNode( sf::IntRect( sf::Vector2i( static_cast<int>( k * tileSets.tileWidth ), static_cast<int>( p * tileSets.tileHeight ) ),
                                                                                 sf::Vector2i( static_cast<int>( tileSets.tileWidth ), static_cast<int>( tileSets.tileHeight ) ) ) ) );
                                rect.get()->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
                                rect.get()->getSprite()->setOutlineThickness( 1 );
                                rect.get()->getSprite()->setOutlineColor( sf::Color(0,0,0,0) );
                                this->mDrawableGrid.back().push_back( rect.get( ) );
                                mSceneGraph.attachChild( std::move( rect ) );
                            }

                        }
                    }
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
                     if( object.type == "LightInfantry" )
                     {
                         std::unique_ptr<Unit> unit( new Unit( mCurrentUnits.size(), UnitDataTable.at( UnitTypeMap.at( "LightInfantry" ) ), mTextures ) );
                         unit->setPosition( object.x, object.y );
                         mMovementGrid.addUnit( mCurrentUnits.size(), object.x, object.y );
                         this->mCurrentUnits.push_back( unit.get( ) );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else if( object.type == "HeavyInfantry" )
                     {
                         std::unique_ptr<Unit> unit( new Unit( mCurrentUnits.size(), UnitDataTable.at( UnitTypeMap.at( "HeavyInfantry" ) ), mTextures ) );
                         unit->setPosition( object.x, object.y );
                         mMovementGrid.addUnit( mCurrentUnits.size(), object.x, object.y );
                         this->mCurrentUnits.push_back( unit.get( ) );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else
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
         *
         *   Particle effects for extra credit!!!!
         *
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
                    std::unique_ptr<ParticleNode> node( new ParticleNode( ParticleMap.at( lua_tostring( L, -1 ) ), mTextures ) );
                    mSceneLayers[LayerMap.at( "TileLayer2" )]->attachChild( std::move( node ) );
                    lua_pop( L, 1 );
                }
            }
            lua_pop( L, 1 ); // Particles table
        }
        lua_close( L );
    */

    // Add network node, if necessary
    if( mNetworkedWorld )
    {
        std::unique_ptr<NetworkNode> networkNode( new NetworkNode( ) );
        mNetworkNode = networkNode.get( );
        mSceneGraph.attachChild( std::move( networkNode ) );
        std::cout << "BuildScene Complete on a networked world!!" << std::endl;
    }

}

void World::loadSaveFile( std::string saveFilePath )
{

}
