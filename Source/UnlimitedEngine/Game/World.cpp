#include "World.hpp"


namespace
{
    const std::vector<UnitTypeData> UnitDataTable = initializeUnitTypeData;  // initializeUnitTypeData in DataTable.hpp
    const std::vector<BuildingData> BuildingDataTable = initializeBuildingTypeData;  // initializeBuildingData in DataTable.hpp
    const std::vector<std::vector<float>> UnitMoveCost = inititializeUnitMovementCostTable;
}

World::World( State::Context* context, StateStack* stack, sf::RenderTarget& outputTarget, FontManager& fonts, SoundPlayer& sounds, unsigned int level, bool networked, bool isLocalMultiplayer )
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
    , mClientTeamColor( Category::Blue )    
    , mBlueTeamStats( nullptr )
    , mRedTeamStats( nullptr )
    , mUpdateFogOfWar( true )
    , mWaitingForPlayer( false )
    , mEndTurn( true )
    , mCurrentUnits( std::vector<Unit*>( ) )
    , mWifeBot( nullptr )
{
    if( !mSceneTexture.create( static_cast<unsigned int>( mTarget.getView( ).getSize( ).x ), mTarget.getView().getSize( ).y ) ) std::cout << "Render ERROR" << std::endl;
    mSceneTexture.setView( mWorldView );
    this->registerStates( );
    buildScene( MediaFileMap.at( "Maps" ).at( level ) );

    if( !mLocalMultiplayerWorld && !mNetworkedWorld )
    {   // Game is a single player game add AI
        mWifeBot = new WifeBot( this, &mCurrentUnits );
    }
}

void World::handleEvent( const sf::Event & event )
{
    mWifeBot->handleEvent( event );
}

void World::handleLeftClick( sf::Vector2i mousePos )
{
    bool flag = false;
    // Find Which Square on the grid the user clicked
    for( unsigned int j = 0; j < mGridHeight; ++j )
    {
        for( unsigned int i = 0; i < mGridWidth; ++i )
        {
            Square* square = &mGrid[j * mGridWidth + i];
            if( square->mBounds.contains( mousePos.x, mousePos.y ) ) // pos refers to the mouse pos when it was clicked.
            {
                flag = true; // break out loop, the square has been found
                if( square->isPossibleNewLocation )
                {
                /*
                    if( getSelectedBuilding() > -1 )  // a spawn was selected before so the player wants to drop a new unit this could be done better
                    {
                        // spawnUnit( static_cast<unsigned int>( mSelectedUnit ), sf::Vector2i( static_cast<int>(i), static_cast<int>(j) ) );
                        if( mNetworkedWorld )
                            mNetworkNode->notifyGameAction( GameActions::Type::SpawnUnit, sf::Vector2f( i, j ) );
                            mCurrentUnits.at( mCurrentUnits.size() - 1 )->mHasMoved = true;
                            clearGrid( );
                    }
                    else
                    */
                    {
                        for( auto u : mCurrentUnits )
                            if( u->mID == mSelectedUnit )
                            {
                                moveUnit( u->mGridIndex, square->gridIndex  );
                                if( !u->mHasSpentAction ) mWaitingForPlayer = false;
                                break;
                            }
                    }
                }
                else if( square->isPossibleAttackPosition  )
                {
                    // An Ability is being used and this square has been selected as the origin
                    Unit* unit = nullptr;
                    for( auto u : mCurrentUnits ) if( u->mID == mSelectedUnit ) { unit = u; break; }
                    if( unit )
                    {
                        unit->mAbilities.at( unit->mSelectedAbility ).origin = square->gridIndex;
                        if( unit->mAbilities.at( unit->mSelectedAbility ).hasRotation ) // if attack must be rotated
                            // Get rotation from player call attack from there
                            mStateStack->pushState( States::RotationSelectMenuState );
                        else
                        {
                             unit->mAbilities.at( unit->mSelectedAbility ).origin = square->gridIndex;
                             for( auto t : unit->mAbilities.at( unit->mSelectedAbility ).AOE.at( "only" ) ) // use abililty on all units inside the ability AOE[0]
                             {
                                 // get all units inside AOE from grid
                                 std::string id = this->mGrid.at( (t.y+square->gridIndex.y) * (this->mGridWidth) + (t.x+square->gridIndex.x) ).unitID;
                                 for( auto u : mCurrentUnits )
                                    if( u->mID == id )
                                    {
                                        unit->useAbility( unit->mSelectedAbility, u );
                                        mWaitingForPlayer = false;
                                        break;
                                    }
                             }
                             this->clearGrid( );
                        }
                    }
                    else std::cout << "World::handleLeftClick( ) ERROR Selected Unit not found!!" << std::endl;
                }
                /*
                else if( square->buildingID > -1 && mCurrentBuildings.at( square->buildingID )->mCategory & mWorld->mCurrentTurn )
                {   // square has a spawn point on it.
                     clearGrid( );
                     setSelectedBuilding(square->buildingID );
                     mStateStack->pushState( States::SpawnPointMenuState );
                     // where to spawn the unit??
                     std::vector<sf::Vector2i> possibleLocations = getPossiblePositions( mCurrentBuildings.at( static_cast<unsigned int>(mWorld->getSelectedBuilding()) )->mGridIndex, static_cast<unsigned int>(mWorld->mSelectedUnit), 1 );
                     for( auto loc : possibleLocations )
                     {
                         mGrid[loc.y * mGridWidth + loc.x].isPossibleNewLocation = true;
                         mGrid[loc.y * mGridWidth + loc.x].rect->getSprite()->setFillColor( sf::Color::Cyan );
                     }
                     mWaitingForPlayer = false;
                }
                */

                break; // break inner (gridWidth) loop
            }
            if( flag ) break; // break outer (gridHeight) loop
        }
    }
}

bool World::update( sf::Time dt )
{
    for( auto u : mCurrentUnits )
    {
        if( u->isDestroyed( ) )
        {
            if( u->mID == mSelectedUnit ) endTurn( );
            removeUnit( u->mGridIndex );
            if( checkVictoryConditions(  ) )
            {
                std::cout << "VictoryConditions have been met!" << std::endl;
                mStateStack->popState( );
                mStateStack->pushState( States::Title );
            }
        }
    }

    mWifeBot->update( dt );

    while( !mCommandQueue.isEmpty( ) ) mSceneGraph.onCommand( mCommandQueue.pop( ), dt );

    if( mUpdateFogOfWar )
    {
        updateFogOfWar( );
        mUpdateFogOfWar = false;
    }
    if( mEndTurn ) endTurn( );

    Unit* unit = nullptr;
    for( auto u : mCurrentUnits ) if( u->mID == mSelectedUnit ) { unit = u;  break; }
    if( unit && !unit->isDestroyed( ) )
    {
        if( unit->mHasMoved && unit->mHasSpentAction && unit->mAnimationTimer == sf::Time::Zero && !unit->isMoving()) endTurn( );
        else if( !mWaitingForPlayer && (!unit->mHasSpentAction || !unit->mHasMoved) && !unit->isMoving() && unit->mAnimationTimer == sf::Time::Zero )
        {
            mWorldView.setCenter( mSelectedGridIndex.x * TILE_SIZE, mSelectedGridIndex.y * TILE_SIZE );
            mDeltaMousePosition = sf::Vector2f( 0,0 );
            mDeltaMousePosition.x -= unit->getPosition().x - WINDOW_WIDTH / 2;
            mDeltaMousePosition.y -= unit->getPosition().y - WINDOW_HEIGHT / 2 + TILE_SIZE / 2;
            mBlueTeamStats->setPosition( unit->getPosition().x - 500, unit->getPosition().y - 350 );
            mRedTeamStats->setPosition( unit->getPosition().x - 500, unit->getPosition().y - 350 );
            if( unit->mCategory & Category::BlueUnit )
            {
                mWaitingForPlayer = true;
                mStateStack->pushState( States::AbilitySelectMenuState );
            }
        }
    }
    else
    {
        std::cout << "The is no unit selected!" << std::endl;
    }
    // The onscreen debugging view of influence set to show offensive influence by default
    // if( mUpdateInfluenceMap ) updateInfluenceMap( );

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
        mBlueTeamStats->move( 0.0f, -1 * mCameraPanSpeed );
        mRedTeamStats->move( 0.0f, -1 * mCameraPanSpeed );
    }
    else if( sf::Keyboard::isKeyPressed( sf::Keyboard::S  ) ) // || sf::Mouse::getPosition().y > WINDOW_HEIGHT - 100 )
    {
        mWorldView.move( 0.0f, mCameraPanSpeed );
        mDeltaMousePosition.y -= mCameraPanSpeed;
        mBlueTeamStats->move( 0.0f, mCameraPanSpeed );
        mRedTeamStats->move( 0.0f, mCameraPanSpeed );
    }
    else if( sf::Keyboard::isKeyPressed( sf::Keyboard::A  ) ) //  || sf::Mouse::getPosition().x < 100 )
    {
        mWorldView.move( -1 * mCameraPanSpeed, 0.0f );
        mDeltaMousePosition.x += mCameraPanSpeed;
        mBlueTeamStats->move( -1 * mCameraPanSpeed, 0.0f );
        mRedTeamStats->move( -1 * mCameraPanSpeed, 0.0f );
    }
    else if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) ) // || sf::Mouse::getPosition().x > WINDOW_WIDTH - 100 )
    {
        mWorldView.move( mCameraPanSpeed, 0.0f );
        mDeltaMousePosition.x -= mCameraPanSpeed;
        mBlueTeamStats->move( mCameraPanSpeed, 0.0f );
        mRedTeamStats->move( mCameraPanSpeed, 0.0f );
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

std::vector<sf::Vector2i> World::getPossiblePositions( sf::Vector2i startingPoint, unsigned int unitType, int distanceLeft )
{
    std::vector<sf::Vector2i> temp;
    buildPossiblePositions( &temp, startingPoint, unitType, distanceLeft );
    return temp;
}

std::vector<sf::Vector2i> World::getPossibleAttackPositions( sf::Vector2i startingPoint, int innerRange, int outerRange )
{
    std::vector<sf::Vector2i> innerList;
    buildPossiblePositions( &innerList, startingPoint, -1, innerRange );

    std::vector<sf::Vector2i> outerList;
    buildPossiblePositions( &outerList, startingPoint, -1, outerRange );

    while( innerList.size() )
    {
        for( auto i = outerList.begin(); i < outerList.end(); ++i )
        {
            if( innerList.back() == *i )
            {
                outerList.erase( i );
                break;
            }
        }
        innerList.pop_back();
    }
    return outerList;
}

void World::spawnUnit( Category::Type unitType, sf::Vector2i gridIndex )
{
    // This method should be left empty untill everything else is working
    // This method refersto units being created after the initial buildScene method
    // Which may or may not exist in the final game.
}

void World::showUnitMovement( unsigned int i, unsigned int j )
{
    std::string id = mGrid[j * mGridWidth + i].unitID;
    Unit* unit;
    for( auto u : mCurrentUnits ) if( u->mID == id ) { unit = u; break; }
    assert( unit );

    if( !unit->mHasMoved ) // units can only move once per turn
    {
        std::vector<sf::Vector2i> possibleLocations = getPossiblePositions(
                                                    sf::Vector2i( static_cast<int>( i ), static_cast<int>( j ) ),
                                                    unit->mUnitType, static_cast<unsigned int>( unit->mSpeed ) );
        for( auto loc : possibleLocations )
        {
            if( !mGrid[loc.y * mGridWidth + loc.x].isOccupied )
            {
                mGrid[loc.y * mGridWidth + loc.x].isPossibleNewLocation = true;
                if( unit->mIsVisible && (( !mNetworkedWorld && !mLocalMultiplayerWorld ) ||
                        mNetworkedWorld || mLocalMultiplayerWorld ) )
                    mGrid[loc.y * mGridWidth + loc.x].rect->getSprite()->setFillColor( sf::Color::Cyan );
            }
        }
        mSelectedGridIndex = sf::Vector2i( i, j );
        mSelectedUnit = unit->mID;
        unit->mIsSelectedUnit = true;
        mWaitingForPlayer = true;
    }
    else
    {
        mStateStack->pushState( States::AbilitySelectMenuState );
        // Play error sound effect or something to indicate the unit cant move
    }
}

void World::updateFogOfWar( void )
{
    if( mUpdateFogOfWar && !mLocalMultiplayerWorld )
        {
            // Fill fog completly
            for( unsigned int i = 0; i < mGrid.size(); ++i )
                mGrid[i].isVisible = true; // CHANGE THIS TO FALSE TO ACTIVATE FOG OF WAR!!!!!!

            // traverse units and buildPos using perception to mark visible squares
            for( auto unit : mCurrentUnits )
            {
                if( unit != nullptr )
                {
                    unit->mIsVisible = false;
                    if( !unit->isDestroyed() && unit->mCategory & Category::Type::Blue )
                    {
                        std::vector<sf::Vector2i> visibleSquares;
                        buildPossiblePositions( &visibleSquares, unit->mGridIndex, -1, unit->mPerception );
                        for( auto loc : visibleSquares )
                            mGrid[loc.y * mGridWidth + loc.x].isVisible = true;
                    }
                    if( mGrid[unit->mGridIndex.y * mGridWidth + unit->mGridIndex.x].isVisible ) unit->mIsVisible = true;
                }
            }

            // update fog based on new data
            for( auto square : mGrid )
                if( !square.isPossibleAttackPosition )
                {
                    if( square.isVisible )
                    {
                        square.rect->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
                        if( square.isOccupied )
                            for( auto u : mCurrentUnits )
                                if( u->mID == square.unitID )
                                {
                                    u->mIsVisible = true;
                                    break;
                                }
                        //else if( square.buildingID > -1 ) mCurrentBuildings.at( square.buildingID )->mIsVisible = true;
                    }
                    else
                    {
                       // if( square.buildingID > -1 ) mCurrentBuildings.at( square.buildingID )->mIsVisible = false;
                        square.rect->getSprite()->setFillColor( sf::Color( 0, 0, 0, 100 ) );
                    }
                }
                else
                {
                    if( square.isOccupied )
                        for( auto u : mCurrentUnits )
                            if( u->mID == square.unitID )
                            {
                                u->mIsVisible = true;
                                break;
                            }
                    // else if( square.buildingID > -1 ) mCurrentBuildings.at( square.buildingID )->mIsVisible = true;
                }
        }
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

                /*
                 * This is for adding particle effects later
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
                    // make sure there is level data to load
                    if( !map.layers[i].data.size() )
                        map.layers[i].data = generateTileMap( map.layers[i].width, map.layers[i].height, 0.01 );

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

                     if( object.type == "Fighter" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedUnit;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueUnit;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Unit> unit( new Unit( std::to_string( mCurrentUnits.size( ) ), category, UnitDataTable.at( UnitTypeMap.at( "Fighter" ) ), mTextures, mFonts ) );
                         unit->setPosition( object.x, object.y - TILE_SIZE - (TILE_SIZE * .5) );
                         unit->mSprite.setTextureRect( UnitDataTable.at( UnitTypeMap.at( "Fighter" ) ).textureRect );
                         addUnit( unit.get() );
                         mCurrentUnits.push_back( unit.get( ) );
                         node.get( )->attachChild( std::move( unit ) );


                         ///
                         /// THIS IS FOR DEBUG ONLY MAKE UNITS DEFINE IMPORTANCE FROM LUA
                         /// ( also must consider dif AI might consider something more/less important.. )
                         ///    - importance is the metric used by wifebot to determine where to move her units.
                         mGrid[object.y/TILE_SIZE * mGridWidth + object.x/TILE_SIZE].importance = 20;

                     }
                     else if( object.type == "Templar" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedUnit;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueUnit;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Unit> unit( new Unit( std::to_string( mCurrentUnits.size( ) ), category, UnitDataTable.at( UnitTypeMap.at( "Templar" ) ), mTextures, mFonts ) );
                         unit->setPosition( object.x, object.y - TILE_SIZE - (TILE_SIZE * .5) );
                         unit->mSprite.setTextureRect( UnitDataTable.at( UnitTypeMap.at( "Templar" ) ).textureRect );
                         addUnit( unit.get() );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else if( object.type == "Archer" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedUnit;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueUnit;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Unit> unit( new Unit( std::to_string( mCurrentUnits.size( ) ), category, UnitDataTable.at( UnitTypeMap.at( "Archer" ) ), mTextures, mFonts ) );
                         unit->setPosition( object.x, object.y - TILE_SIZE - (TILE_SIZE * .5) );
                         unit->mSprite.setTextureRect( UnitDataTable.at( UnitTypeMap.at( "Archer" ) ).textureRect );
                         addUnit( unit.get() );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else if( object.type == "Medic" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedUnit;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueUnit;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Unit> unit( new Unit( std::to_string( mCurrentUnits.size( ) ), category, UnitDataTable.at( UnitTypeMap.at( "Medic" ) ), mTextures, mFonts ) );
                         unit->setPosition( object.x, object.y - TILE_SIZE - (TILE_SIZE * .5) );
                         unit->mSprite.setTextureRect( UnitDataTable.at( UnitTypeMap.at( "Medic" ) ).textureRect );
                         addUnit( unit.get() );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else if( object.type == "Wizard" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedUnit;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueUnit;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Unit> unit( new Unit( std::to_string( mCurrentUnits.size( ) ), category, UnitDataTable.at( UnitTypeMap.at( "Wizard" ) ), mTextures, mFonts ) );
                         unit->setPosition( object.x, object.y - TILE_SIZE - (TILE_SIZE * .5) );
                         unit->mSprite.setTextureRect( UnitDataTable.at( UnitTypeMap.at( "Wizard" ) ).textureRect );
                         addUnit( unit.get() );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else if( object.type == "Rogue" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedUnit;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueUnit;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Unit> unit( new Unit( std::to_string( mCurrentUnits.size( ) ), category, UnitDataTable.at( UnitTypeMap.at( "Rogue" ) ), mTextures, mFonts ) );
                         unit->setPosition( object.x, object.y - TILE_SIZE - (TILE_SIZE * .5) );
                         unit->mSprite.setTextureRect( UnitDataTable.at( UnitTypeMap.at( "Rogue" ) ).textureRect );
                         addUnit( unit.get() );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else if( object.type == "Scout" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedUnit;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueUnit;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Unit> unit( new Unit( std::to_string( mCurrentUnits.size( ) ), category, UnitDataTable.at( UnitTypeMap.at( "Scout" ) ), mTextures, mFonts ) );
                         unit->setPosition( object.x, object.y - TILE_SIZE - (TILE_SIZE * .5) );
                         unit->mSprite.setTextureRect( UnitDataTable.at( UnitTypeMap.at( "Scout" ) ).textureRect );
                         addUnit( unit.get() );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else if( object.type == "Bard" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedUnit;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueUnit;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Unit> unit( new Unit( std::to_string( mCurrentUnits.size( ) ), category, UnitDataTable.at( UnitTypeMap.at( "Bard" ) ), mTextures, mFonts ) );
                         unit->setPosition( object.x, object.y - TILE_SIZE - (TILE_SIZE * .5) );
                         unit->mSprite.setTextureRect( UnitDataTable.at( UnitTypeMap.at( "Bard" ) ).textureRect );
                         addUnit( unit.get() );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else if( object.type == "TacticalLocation" )
                     {
                         // set value of importance property to the tile found at the same location as this object
                         mGrid[object.y/TILE_SIZE * mGridWidth + object.x/TILE_SIZE].importance = std::atof( object.properties.at( "Importance" ).c_str( ) );
                         mGrid[object.y/TILE_SIZE * mGridWidth + object.x/TILE_SIZE].debugText->setString( object.properties.at( "Importance" ).c_str( ) )  ;
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
        // Add HUD
        std::unique_ptr<TeamStats> mBlueTeam( new TeamStats( Category::TeamBlue, mFonts ) );
        mBlueTeamStats = mBlueTeam.get();
        mBlueTeamStats->setPosition( mWorldView.getViewport().left, mWorldView.getViewport().top );
        mSceneGraph.attachChild( std::move( mBlueTeam ) );
        std::unique_ptr<TeamStats> mRedTeam( new TeamStats( Category::TeamRed, mFonts ) );
        mRedTeamStats = mRedTeam.get();
        mRedTeamStats->setPosition( mWorldView.getViewport().left, mWorldView.getViewport().top );
        mSceneGraph.attachChild( std::move( mRedTeam ) );
        mRedTeamStats->mDisplay = false;
        mBlueTeamStats->mDisplay = true;

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

    for( unsigned int i = 0; i < height; ++i ) // y
        for( unsigned int j = 0; j < width; ++j ) // x
        {
            double x = (double)j / (double)width;
            double y = (double)i / (double)height;
            unsigned int value = ( pn.noise( x, y, increment ) ) * 100;
            std::cout << value << std::endl;

            if( value < 20 )
                tileMap.push_back( 3 );
            else if( value < 50 )
                tileMap.push_back( 1 );
            else if( value < 60 )
                tileMap.push_back( 2 );
            else if( value < 80 )
                tileMap.push_back( 4 );
            else if( value < 100 )
                tileMap.push_back( 5 );
            else
                tileMap.push_back( 1 );
        }
    return tileMap;
}


void World::registerStates( )
{
    mStateStack->registerState<SpawnPointMenuState>( States::SpawnPointMenuState, this );
    mStateStack->registerState<AbilitySelectMenuState>( States::AbilitySelectMenuState, this );
    mStateStack->registerState<RotationSelectMenuState>( States::RotationSelectMenuState, this );
    mStateStack->registerState<WaitMenu>( States::WaitMenuState, this );
}

void World::addUnit( Unit* unit )
{
    for( unsigned int i = 0; i < mGrid.size(); ++i )
    {
        if( mGrid[i].mBounds.contains( unit->getPosition().x, unit->getPosition().y + TILE_SIZE / 2 ) )
        {
            unit->mGridIndex = mGrid[i].gridIndex;
            std::cout << "AddUnit() to index " << mGrid[i].gridIndex.x << ", " << mGrid[i].gridIndex.y << std::endl;
            mGrid[i].isOccupied = true;
            mGrid[i].unitID = unit->mID;
            // mData[i].debugText->setString( std::to_string( mData[i].unitID ) );
            // mData[i].debugText->setColor( sf::Color::Yellow );
            break;
        }
    }
}

void World::removeUnit( sf::Vector2i position )
{
    std::string id = mGrid[position.y * mGridWidth + position.x].unitID;
    std::cout << "Unit being removed: " << id << std::endl;
    for( unsigned int i = 0; i < mCurrentUnits.size(); ++i ) if( mCurrentUnits[i]->mID == id ) { mCurrentUnits.erase( mCurrentUnits.begin( ) + i ); break; }
    mGrid[position.y * mGridWidth + position.x].unitID = "";
    mGrid[position.y * mGridWidth + position.x].isOccupied = false;
    mSceneGraph.removeWrecks( );
    mUpdateFogOfWar = true;
}

bool World::moveUnit( sf::Vector2i currentPos, sf::Vector2i newPos )
{
    Square* newSquare = &mGrid[newPos.y * mGridWidth + newPos.x];
    Square* oldSquare = &mGrid[currentPos.y * mGridWidth + currentPos.x];
    if( newSquare->isOccupied ) clearGrid();
    else
    {
        oldSquare->isVisible = false;
        newSquare->isVisible = true;
        newSquare->unitID = oldSquare->unitID;
        newSquare->isOccupied = true;
        Unit* unit;
        for( auto u : mCurrentUnits ) if( u->mID == oldSquare->unitID ) unit = u;
        oldSquare->unitID = -1;
        oldSquare->isOccupied = false;
        newSquare->isOccupied = true;
        unit->mGridIndex = newPos;
        mSelectedGridIndex = newPos;
        unit->addModifier( StatModifier( "initiative", "20-45", 1 ) );
        // std::cout << "Grid::MoveUnit Unit Path:" << std::endl;
        unit->mPath = new PathFinder<Square>( mGrid, sf::Vector2i( mGridWidth, mGridHeight ), oldSquare, newSquare,
                [=]( Square* start, Square* goal )->float {
                 float distance = std::abs( start->gridIndex.x - goal->gridIndex.x )+std::abs( start->gridIndex.y - goal->gridIndex.y );
                     if( distance > 1 )
                     {
                         distance *= .5;
                         distance += getMoveCost( unit->mUnitType, start->terrainType );
                     }
                     return distance;

                 } );

        // remove movement squares
        for( unsigned int i = 0; i < mGrid.size(); ++i )
        {
            mGrid[i].isPossibleNewLocation = false;
            mGrid[i].rect->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
        }
        mUpdateFogOfWar = true;
    }
    return true;
}

void World::getTartgets( std::vector<sf::Vector2i> possibleAttackLocations )
{
    bool targetExists = false;
    for( auto loc : possibleAttackLocations )
        if( static_cast<unsigned int>( loc.y * mGridWidth + loc.x ) < mGrid.size() )
        {
            Square* location =  &mGrid[static_cast<unsigned int>( loc.y * mGridWidth + loc.x )];
            if( location->isOccupied )
            {
                Unit* currentUnit = nullptr;
                for( auto unit : mCurrentUnits ) if( unit->mID == mSelectedUnit ) { currentUnit = unit; break; }
                for( auto unit : mCurrentUnits )
                    if( unit->mID == location->unitID && !( unit->getCategory() & currentUnit->getCategory( ) ) )
                        {
                            location->isPossibleAttackPosition = true;
                            location->rect->getSprite( )->setFillColor( sf::Color( 255, 165, 0, 255 ) );
                            targetExists = true;
                            break;
                        }
            }
        }
    if( !targetExists )
    {
        // get current unit
        Unit* unit = nullptr;
        for( auto u : mCurrentUnits ) if( u->mIsSelectedUnit ) { unit = u; break; }
        if( unit != nullptr )
        {
            mWorldView.setCenter( mSelectedGridIndex.x * TILE_SIZE, mSelectedGridIndex.y * TILE_SIZE );
            mDeltaMousePosition = sf::Vector2f( 0, 0 );
            mDeltaMousePosition.x -= unit->getPosition( ).x - WINDOW_WIDTH / 2;
            mDeltaMousePosition.y -= unit->getPosition( ).y - WINDOW_HEIGHT / 2 + TILE_SIZE / 2;
            mBlueTeamStats->setPosition( unit->getPosition( ).x - 500, unit->getPosition( ).y - 350 );
            mRedTeamStats->setPosition( unit->getPosition( ).x - 500,  unit->getPosition( ).y - 350 );
            mStateStack->pushState( States::AbilitySelectMenuState );
        }
        else std::cout << "Error in World::getTargets() currently selected Unit not found." << std::endl;
    }
    mWaitingForPlayer = true;
}

void World::clearGrid( void )
{
    for( unsigned int i = 0; i < mGrid.size(); ++i )
    {
        mGrid[i].isPossibleNewLocation = false;
        mGrid[i].isPossibleAttackPosition = false;
        mGrid[i].rect->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
        // clear influence map before update
        mGrid[i].offensiveInfluence = 0.0f;
        mGrid[i].defensiveInfluence = 0.0f;
        mGrid[i].tacticalInfluence = 0.0f;
    }
    mUpdateFogOfWar = true;
}

void World::buildPossiblePositions( std::vector<sf::Vector2i>* mPossiblePositions, sf::Vector2i startingPoint, unsigned int unitType, int distanceLeft )
{
    if( distanceLeft >= 0 && static_cast<unsigned int>( startingPoint.y * mGridWidth + startingPoint.x ) < mGrid.size() )
    {
        Square* square = &mGrid[startingPoint.y * mGridWidth + startingPoint.x];
        mPossiblePositions->push_back( startingPoint );
        distanceLeft -= getMoveCost( unitType, square->terrainType );

        buildPossiblePositions( mPossiblePositions, sf::Vector2i( startingPoint.x, startingPoint.y - 1 ), unitType, distanceLeft );
        buildPossiblePositions( mPossiblePositions, sf::Vector2i( startingPoint.x, startingPoint.y + 1 ), unitType, distanceLeft );

        if( square != &mGrid[startingPoint.y * mGridWidth + 0] ) // square != farthest left square in a row
            buildPossiblePositions( mPossiblePositions, sf::Vector2i( startingPoint.x - 1, startingPoint.y ), unitType, distanceLeft );
        if( square != &mGrid[startingPoint.y * mGridWidth + (mGridWidth-1)] ) // square != farthest right squre in a row
            buildPossiblePositions( mPossiblePositions, sf::Vector2i( startingPoint.x + 1, startingPoint.y ), unitType, distanceLeft );
    }
}

unsigned int World::getMoveCost( int unitType, unsigned int terrainType )
{
    if( unitType == -1 ) return 1;
    return UnitMoveCost[unitType][terrainType];
}

void World::endTurn( void )
{
    mEndTurn = false;
    Unit* next = nullptr;
    if( mCurrentUnits.size() > 1 )
    {
        while( true )
        {
            for( auto u : mCurrentUnits )
               if(  u && !u->isDestroyed( ) && !u->mWasTheLastUnit && ( next == nullptr || ( next->mInitiative < u->mInitiative ) ) ) next = u;
            if( next )
            {
                if( next->mInitiative < 100 ) for( auto u : mCurrentUnits ) u->mInitiative += randomInt( u->mSpeed * 5 ) + u->mSpeed * 2;
                else
                {
                    // clear unit stats for a fresh turn here
                    for( auto u : mCurrentUnits ) if( u->mID == mSelectedUnit ) { u->mWasTheLastUnit = false; break; }
                    mSelectedUnit = next->mID;
                    next->mHasMoved = false;
                    next->mHasSpentAction = false;
                    next->mBeginingOfTurn = true;
                    next->mIsSelectedUnit = true;
                    next->mWasTheLastUnit = true;
                    mSelectedGridIndex = sf::Vector2i( next->mGridIndex.x, next->mGridIndex.y );
                    mWorldView.setCenter( mSelectedGridIndex.x * TILE_SIZE, mSelectedGridIndex.y * TILE_SIZE );
                    mDeltaMousePosition = sf::Vector2f( 0, 0 );
                    mDeltaMousePosition.x -= next->getPosition( ).x - WINDOW_WIDTH / 2;
                    mDeltaMousePosition.y -= next->getPosition( ).y - WINDOW_HEIGHT / 2 + TILE_SIZE / 2;
                    mBlueTeamStats->setPosition( next->getPosition( ).x - 500, next->getPosition( ).y - 350 );
                    mRedTeamStats->setPosition( next->getPosition( ).x - 500,  next->getPosition( ).y - 350 );

                    if( mWifeBot == nullptr || next->mCategory & Category::BlueUnit )
                    {
                        mWaitingForPlayer = true;
                        mStateStack->pushState( States::AbilitySelectMenuState );
                    }
                    else if( next->mCategory & Category::RedUnit )
                    {
                        // std::cout << "The wife bot should be called from here to control the units turn" << std::endl;
                        mWaitingForPlayer = false;
                        mWifeBot->mRecalculate = true;
                    }
                    break;
                }
            }
            else // There are no units to change to call the end of game
            {
                mStateStack->popState( );
                mStateStack->pushState( States::Menu );
                std::cout << "EndTurn found no possible units to turn to, the game is over." << std::endl;
                break;
            }
        }
    }
    else
    {
        mStateStack->popState( );
        mStateStack->pushState( States::Menu );
        std::cout << "EndTurn found no possible units to turn to, the game is over." << std::endl;
    }
}

bool World::checkVictoryConditions( void )
{
    unsigned int numBlueUnits = 0;
    unsigned int numRedUnits = 0;
    for( auto unit : mCurrentUnits )
        if( unit->getCategory( ) & Category::RedUnit ) numRedUnits++;
        else if( unit->getCategory( ) & Category::BlueUnit ) numBlueUnits++;
    return !( numBlueUnits > 0 && numRedUnits > 0 );
}
