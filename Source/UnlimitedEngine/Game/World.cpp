#include "World.hpp"


namespace
{
    // left i for examples of how to get tables from DataTables.hpp

    //const std::vector<UnitTypeData> UnitDataTable = initializeUnitTypeData;  // initializeUnitTypeData in DataTable.hpp
    //const std::vector<BuildingData> BuildingDataTable = initializeBuildingTypeData;  // initializeBuildingData in DataTable.hpp
    //const std::vector<std::vector<float>> UnitMoveCost = inititializeUnitMovementCostTable;
}

World::World( State::Context* context, StateStack* stack, sf::RenderTarget& outputTarget, FontManager& fonts, SoundPlayer& sounds, bool networked, bool isLocalMultiplayer )
    : mTarget( outputTarget )
    , mSceneTexture( )
    , mWorldView( mTarget.getDefaultView( ) )
    , mTextures( )
    , mFonts( fonts )
    , mSounds( sounds )
    , mWindowSprite(  )
    , mSceneGraph( )
    , mSceneLayers( )
    , mDeltaMousePosition( 0, 0 )
    , mCameraPanSpeed( 8 )
    , mNetworkedWorld( networked )
    , mLocalMultiplayerWorld( isLocalMultiplayer )
    , mNetworkNode( nullptr )
    , mWorldContext( *context )
    , mStateStack( stack )
    // GAME ATTRIBUTES
    , mPlayerAvatar( nullptr )
{
    if( !mSceneTexture.create( static_cast<unsigned int>( mTarget.getView( ).getSize( ).x ), mTarget.getView().getSize( ).y ) ) std::cout << "Render ERROR" << std::endl;
    mSceneTexture.setView( mWorldView );
    this->registerStates( );
    buildScene( );
}

void World::handleEvent( const sf::Event& )
{
}

bool World::update( sf::Time dt )
{
    try{ mSceneGraph.update( dt, mCommandQueue ); }
    catch( std::exception& e ) { std::cout << "There was an exception in the SceneGraph update: " << e.what( ) << std::endl; }
    /*
        try{ handleCollisions( ); }
        catch( std::exception& e ) {
            std::cout << "There was an exception during the collision update: " << e.what( ) << "\nDo all your map layer names in lua match from tiled?" << std::endl;
        }
    */
    // Update the view
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) ) // || sf::Mouse::getPosition().y < 100 )
    {
        mWorldView.move( 0.0f, -1 * mCameraPanSpeed );
        mDeltaMousePosition.y += mCameraPanSpeed;
    }
    else if( sf::Keyboard::isKeyPressed( sf::Keyboard::S  ) ) // || sf::Mouse::getPosition().y > WINDOW_HEIGHT - 100 )
    {
        mWorldView.move( 0.0f, mCameraPanSpeed );
        mDeltaMousePosition.y -= mCameraPanSpeed;
    }
    else if( sf::Keyboard::isKeyPressed( sf::Keyboard::A  ) ) //  || sf::Mouse::getPosition().x < 100 )
    {
        mWorldView.move( -1 * mCameraPanSpeed, 0.0f );
        mDeltaMousePosition.x += mCameraPanSpeed;
    }
    else if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) ) // || sf::Mouse::getPosition().x > WINDOW_WIDTH - 100 )
    {
        mWorldView.move( mCameraPanSpeed, 0.0f );
        mDeltaMousePosition.x -= mCameraPanSpeed;
    }
    return true;
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

CommandQueue& World::getCommandQueue( )
{
    return mCommandQueue;
}

bool World::pollGameAction( GameActions::Action& out )
{
    return mNetworkNode->pollGameAction( out );
}

void World::buildScene(  )
{
    // NON tiled levels
    // Create layers...
    // Object layer
    std::unique_ptr<SceneNode> objectLayer( new SceneNode( Category::ObjectLayer ) );

    // Add Player avatar

    std::unique_ptr<SpriteNode> playerAvatar( new SpriteNode( mTextures.get( TextureMap.at( "PlayerAvatar" ) ) ) );
    playerAvatar->setPosition( 400, 500 );
    mPlayerAvatar = playerAvatar.get( );
    objectLayer.get( )->attachChild( std::move( playerAvatar ) );

    // Add Wall sprites

    // Add Theme art.

    mSceneLayers.push_back( objectLayer.get( ) );
    mSceneGraph.attachChild( std::move( objectLayer ) );
    // Create objects in the world
    // Players
    /*
    // load TiledMap
        Tiled::TiledMap map = Tiled::loadFromFile( tileMapFilePath ); //mContext.TiledMapFilePath );
        int TILE_SIZE = 64;

        struct Tile {
            std::string texID;
            sf::Rect<int> rect;
        };
        std::vector<Tile> tiles = std::vector<Tile>( );
        tiles.push_back( Tile() ); // Add in the first tile as an empty space because the gId in tiled starts at 1
        tiles[0].texID = "NONE";
        tiles[0].rect = sf::Rect<int>( 0, 0, 64, 64 ); // HARD VALUES THAT NEED TO BE REMOVED DO NOT REMOVE ME UNTILL ITS DONE!!!!!!!!!!!!!!
                                                            // Should be able to get these values from tiled file anyway....
        for( unsigned int i = 0; i < map.tileSets.size(); ++i )
        {
            // define loops to divide up image
            int y = 0;
            int x = 0;
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
                    tile.rect = sf::Rect<int>( x, y, TILE_SIZE, TILE_SIZE );
                    // add tile to set of possible tiles to use later it's index in vector is it's id as found in testMap.lua
                    tiles.push_back( tile );
                    x += TILE_SIZE;
                }
                x = 0;
                y += TILE_SIZE;
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
                ///
                /// WHY? just use one image per map....
                auto tileSets = map.tileSets[0];
                std::cout << "Currently loading tileLayer from tileSet: " << tileSets.name << " This is hardcoded and needs to be fixed!" << std::endl;
                std::cout << "Particle Effects are being attached in a hardcoded way, NEEDS TO BE IMPROVED!" << std::endl;

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
                {
                    // make sure there is level data to load
                    //if( !map.layers[i].data.size() )
                        //map.layers[i].data = generateTileMap( map.layers[i].width, map.layers[i].height, 0.15 );
                    std::unique_ptr<VertexArrayNode> layer( new VertexArrayNode(  ) );
                    if( !layer.get()->load( mTextures.get( TextureMap.at( map.tileSets[0].name ) ), sf::Vector2u( tileSets.tileWidth, tileSets.tileHeight ), map.layers[i].data, map.width, map.height ) )
                        std::cout << "ERROR loading TiledMap! BuildScene ln: 249" << std::endl;
                    else
                    {
                            mGridWidth = map.layers[i].width;
                            mGridHeight = map.layers[i].height;

                            mSceneLayers.push_back( layer.get( ) );
                            unsigned int counter = 0;
                            for( unsigned int j = 0; j < mGridHeight; ++j )
                            {
                                for( unsigned int i = 0; i < mGridWidth; ++i )
                                {
                                    mGrid.push_back( Square( TerrainTypeMap.at( tileSets.tiles.at( map.layers[0].data[counter]-1 ).at( "type" ) ),
                                                                    sf::Vector2i( i, j ), sf::Vector2f( i * TILE_SIZE, j * TILE_SIZE), TILE_SIZE, false ) );
                                    mGrid.back().gridIndex = sf::Vector2i( i, j );
                                    std::unique_ptr<RectangleShapeNode> rect(
                                                new RectangleShapeNode( sf::IntRect( sf::Vector2i( static_cast<int>( i * tileSets.tileWidth ),
                                                                                                   static_cast<int>( j * tileSets.tileHeight ) ),
                                                                                     sf::Vector2i( static_cast<int>( tileSets.tileWidth ),
                                                                                                   static_cast<int>( tileSets.tileHeight ) ) ) ) );
                                    rect.get( )->getSprite( )->setFillColor( sf::Color( 0, 0, 0, 0 ) );
                                    rect.get( )->getSprite( )->setOutlineThickness( 1 );
                                    rect.get( )->getSprite( )->setOutlineColor( sf::Color( 0, 0, 0, 10 ) );

                                    ++counter;
                                    std::unique_ptr<TextNode> debugText( new TextNode( mFonts, "" ) );
                                    debugText.get( )->setPosition( rect.get( )->getSprite()->getPosition().x + 25, rect.get()->getSprite()->getPosition().y + 25 );
                                    mGrid.back( ).debugText = debugText.get( );
                                    mGrid.back( ).rect = rect.get( );
                                    layer->attachChild( std::move( rect ) );
                                    layer->attachChild( std::move( debugText ) );
                                }
                            }
                            // turn off fog of war if both players are using the same screen.
                            if( mLocalMultiplayerWorld )
                                for( unsigned int i = 0; i < mGrid.size(); ++i )
                                    mGrid[i].isVisible = true;
                            mSceneGraph.attachChild( std::move( layer ) );
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
    */

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


        // Add HUD


        // Add network node, if necessary
        if( mNetworkedWorld )
        {
            std::unique_ptr<NetworkNode> networkNode( new NetworkNode( ) );
            mNetworkNode = networkNode.get( );
            mSceneGraph.attachChild( std::move( networkNode ) );
            std::cout << "BuildScene Complete on a networked world!!" << std::endl;
        }
        else
            std::cout << "BuildScene Complete!!" << std::endl;
}
std::vector<unsigned int> World::generateTileMap( unsigned int width, unsigned int height, double increment )
{
    // Create Perlin Graph to base level on
    PerlinNoise pn( randomInt( 1000 ) );
    std::vector<unsigned int> tileMap;

    // build base terrain
    for( unsigned int i = 0; i < height; ++i ) // y
        for( unsigned int j = 0; j < width; ++j ) // x
        {
            double x = (double)j / (double)width;
            double y = (double)i / (double)height;
            unsigned int value = ( ( ( pn.noise( x, y, increment ) ) * 10 ) );
            //std::cout << value << std::endl;

            if( value < 3 )
                tileMap.push_back( 4 );
            else if( value < 5 )
                tileMap.push_back( 2 );
            else if( value < 8 )
                tileMap.push_back( 1 ); // this should be a 1
            else if( value < 10 )
                tileMap.push_back( 4 );
            else
                tileMap.push_back( 1 ); // this should be a 1
        }
    // add roads
    int numRoads = 3;
    bool hasShifted = false;
    bool vertRoadAdded = false;
    bool horRoadAdded = false;
    while( randomInt( --numRoads ) )
    {
        vertRoadAdded = true;
        // add vertical road
        unsigned int xPos = randomInt( width - 4 ) + 2;
        for( unsigned int i = 0; i < height; ++i )
        {
            if( !hasShifted && i > 0 && xPos < width && xPos > 0 && !randomInt( 4 ) )
            {
                if( randomInt( 2 ) ) xPos++; else xPos--;
                tileMap[(i-1) * width + xPos] = 3;
                hasShifted = true;
            }
            else hasShifted = false;
            if( tileMap[i * width + xPos] != 4 )
                tileMap[i * width + xPos] = 3;
        }
    }
    numRoads++;
    while( randomInt( --numRoads ) )
    {
        horRoadAdded = true;
        // add horizontal road
        unsigned int yPos = randomInt( height - 4 ) + 2;

        for( unsigned int i = 0; i < width; ++i )
        {
            if( !hasShifted && yPos < height && yPos > 0 && !randomInt( 4 ) )
            {
                if( randomInt( 2 ) ) yPos++; else yPos--;
                tileMap[yPos * width + i-1] = 3;
                hasShifted = true;
            }
            else hasShifted = false;
            if( tileMap[yPos * width + i] != 4 )
                tileMap[yPos * width + i] = 3;
        }
    }
    // Walls
    if( !vertRoadAdded && horRoadAdded && !randomInt( 2 ) )
    {
        // add vetical wall
        int xPos = randomInt( width - 6 ) + 3;
        for( unsigned int i = 0; i < height; ++i )
        {
            // change tile at pos to wall unless that tile is a road or water
            int tile = tileMap[i * width + xPos];
            if( tile != 4 && tile != 3 )
                tileMap[i * width + xPos] = 3;
        }
    }
    else if( vertRoadAdded && !horRoadAdded && !randomInt( 2 ) )
    {
        // add horizontal wall
        int yPos = randomInt( height - 6 ) + 3;
        for( unsigned int i = 0; i < width; ++i )
        {
            // change tile at pos to wall unless that tile is a road or water
            int tile = tileMap[yPos * width + i];
            if( tile != 4 && tile != 3 )
                tileMap[yPos * width + i] = 3;
        }
    }

    // Ensure all areas of map are reachable!
    // ( water cant be allowed to cut of corner of map. )
        // A* (ish) to walk grid from place to all other places?


    return tileMap;
}


void World::registerStates( )
{

}



