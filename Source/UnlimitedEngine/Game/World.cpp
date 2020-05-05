#include "World.hpp"
#include "Graphics/VertexArrayNode.hpp"
#include "Tiled/TiledManager.hpp"
#include "Core/Utility.hpp"
#include "Gui/Button.hpp"
#include "States/EndTurnMenuState.hpp"
#include "States/SpawnPointMenuState.hpp"
#include "States/ActionMenuState.hpp"
#include "States/AbilitySelectMenuState.hpp"
#include "States/RotationSelectMenuState.hpp"
#include "States/WaitMenu.hpp"
#include "Core/GameServer.hpp"
#include "Game/WifeBot.hpp"
#include "Graphics/TextNode.hpp"
#include <SFML/Network.hpp>
#include "Core/Globals.hpp"

enum Layers {
    ObjectLayer = 0
};

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
    , mMovementGrid( nullptr )
    , mLevel( level )
    , mSelectedBuilding( -1 )
    , mCurrentTurn( Category::Blue )
    , mClientTeamColor( Category::Blue )
    , mChangeTurnText( "Blue\nTeam's turn!" , mFonts.get( FontMap.at( "Default" ) ), 72 )
    , mChangeTurnTextTimer( sf::Time::Zero )
    , mBlueTeamStats( nullptr )
    , mRedTeamStats( nullptr )
{
    if( !mSceneTexture.create( static_cast<unsigned int>( mTarget.getView( ).getSize( ).x ), mTarget.getView().getSize( ).y ) ) std::cout << "Render ERROR" << std::endl;
    // mWorldView.zoom( 2.0 );
    // mWorldView.move( WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 );
    mSceneTexture.setView( mWorldView );
    this->registerStates( );
    buildScene( MediaFileMap.at( "Maps" ).at( mLevel ) );
    centerOrigin( mChangeTurnText );
    mChangeTurnText.setPosition( WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 200 );
    mChangeTurnText.setOutlineThickness( 3 );
    mChangeTurnText.setOutlineColor( sf::Color( 79, 67, 174, 255 ) );
    mChangeTurnText.setFillColor( sf::Color( 181, 182, 228, 255 ) );
}

World::~World( void )
{
}

void World::draw( )
{
    if( !PostEffect::isSupported( ) )
    {
        mSceneTexture.clear( sf::Color( 0, 0, 0 ) );
        mSceneTexture.setView( mWorldView );
        mSceneTexture.draw( mSceneGraph );
        if( mChangeTurnTextTimer > sf::Time::Zero )
        {
            mChangeTurnText.setPosition( WINDOW_WIDTH / 2 + mDeltaMousePosition.x, WINDOW_HEIGHT / 2 - 200 + mDeltaMousePosition.y );
            mSceneTexture.draw( mChangeTurnText );
        }
        mSceneTexture.display( );
        mWindowSprite.setTexture( mSceneTexture.getTexture( ) );
        mTarget.draw( sf::Sprite( mWindowSprite ) );
        mBloomEffect.apply( mSceneTexture, mTarget );
    }
    else
    {
        mTarget.setView( mWorldView );
        mTarget.draw( mSceneGraph );
        if( mChangeTurnTextTimer > sf::Time::Zero )
        {
            mChangeTurnText.setPosition( WINDOW_WIDTH / 2 + mDeltaMousePosition.x, WINDOW_HEIGHT / 2 - 200 + mDeltaMousePosition.y );
            mTarget.draw( mChangeTurnText );
        }
    }
}

bool World::update( sf::Time dt )
{
    while( !mCommandQueue.isEmpty( ) ) mSceneGraph.onCommand( mCommandQueue.pop( ), dt );
    try{ mSceneGraph.update( dt, mCommandQueue ); }
    catch( std::exception& e ) { std::cout << "There was an exception in the SceneGraph update: " << e.what( ) << std::endl; }
    /*
        try{ handleCollisions( ); }
        catch( std::exception& e ) {
            std::cout << "There was an exception during the collision update: " << e.what( ) << "\nDo all your map layer names in lua match from tiled?" << std::endl;
        }
    */
    mSceneGraph.removeWrecks( );
    if( mChangeTurnTextTimer > sf::Time::Zero )
        mChangeTurnTextTimer -= dt;

    // Update the view
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) ) // || sf::Mouse::getPosition().y < 100 )
    {
        mWorldView.move( 0.0f, -1 * mCameraPanSpeed );
        mDeltaMousePosition.y -= mCameraPanSpeed;
        mBlueTeamStats->move( 0, -mCameraPanSpeed );
        mRedTeamStats->move( 0, -mCameraPanSpeed );
    }
    else if( sf::Keyboard::isKeyPressed( sf::Keyboard::Down  ) ) // || sf::Mouse::getPosition().y > WINDOW_HEIGHT - 100 )
    {
        mWorldView.move( 0.0f, mCameraPanSpeed );
        mDeltaMousePosition.y += mCameraPanSpeed;
        mBlueTeamStats->move( 0, mCameraPanSpeed );
        mRedTeamStats->move( 0, mCameraPanSpeed );
    }
    else if( sf::Keyboard::isKeyPressed( sf::Keyboard::Left  ) ) //  || sf::Mouse::getPosition().x < 100 )
    {
        mWorldView.move( -1 * mCameraPanSpeed, 0.0f );
        mDeltaMousePosition.x -= mCameraPanSpeed;
        mBlueTeamStats->move( -mCameraPanSpeed, 0 );
        mRedTeamStats->move( -mCameraPanSpeed, 0 );
    }
    else if( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) ) // || sf::Mouse::getPosition().x > WINDOW_WIDTH - 100 )
    {
        mWorldView.move( mCameraPanSpeed, 0.0f );
        mDeltaMousePosition.x += mCameraPanSpeed;
        mBlueTeamStats->move( mCameraPanSpeed, 0 );
        mRedTeamStats->move( mCameraPanSpeed, 0 );
    }

    return true;
}

CommandQueue& World::getCommandQueue( ) { return mCommandQueue; }

bool World::pollGameAction( GameActions::Action& out ) { return mNetworkNode->pollGameAction( out ); }

bool World::matchesCategories( std::pair<SceneNode*, SceneNode*>& colliders, Category::Type type1, Category::Type type2 )
{
    unsigned int category1 = colliders.first->getCategory( );
    unsigned int category2 = colliders.second->getCategory( );
    // Make sure first pair entry has category type1 and second has type2
    if( type1 & category1 && type2 & category2 ) return true;
    else if( type1 & category2 && type2 & category1 )
    {
        std::swap( colliders.first, colliders.second );
        return true;
    }
    return false;
}

void World::handleEvent( const sf::Event& event )
{
    if( ( !this->mLocalMultiplayerWorld && mCurrentTurn & Category::Blue ) || this->mLocalMultiplayerWorld )
        if( event.type == sf::Event::KeyReleased &&
                ( event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift ) )
            mStateStack->pushState( States::EndTurnMenuState );
}

void World::changeTurn( void )
{
    if( mCurrentTurn & Category::Red )
    {
        mCurrentTurn = Category::Blue;
        mChangeTurnText.setString( "\t  BLUE\nPLAYER GO!" );
        mBlueTeamStats->mDisplay = true;
        mRedTeamStats->mDisplay = false;
        // solve any end-of-turn buffs etc for red team here
        // solve any start-of-turn bufs etc for blue team here
    }
    // == Cateogry::Blue
    else
    {
        mCurrentTurn = Category::Red;
        mChangeTurnText.setString( "\t   RED\nPLAYER GO!" );
        mBlueTeamStats->mDisplay = false;
        mRedTeamStats->mDisplay = true;
    }
    mChangeTurnTextTimer = sf::milliseconds( 2000 );

    // reset all units temp variables
    mMovementGrid->clearGrid( );
    for( auto unit : mMovementGrid->mCurrentUnits )
    {
        if( unit.second )
        {
            unit.second->mHasMoved = false;
            unit.second->mHasSpentAction = false;
        }
    }

    if( !mNetworkedWorld && !mLocalMultiplayerWorld && (mCurrentTurn & Category::Red) ) // Game is Player vs AI bot is always red
    {
        // Must tell Wifebot to update at the beggining of her turn
        Command com;
        com.category = Category::WifeBot;
        com.action = derivedAction<WifeBot>( [] ( WifeBot& bot, sf::Time ){ bot.recalculate( true ); } );
        mCommandQueue.push( com );
    }

}

void World::spawnUnit( unsigned int unitType, sf::Vector2i gridIndex )
{
    // add unit to grid 
    Category::Type category = Category::None;
    if( mCurrentTurn == Category::Red ) category = Category::RedUnit;
    else if( mCurrentTurn == Category::Blue ) category = Category::BlueUnit;
    else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

    std::unique_ptr<Unit> unit( new Unit( mMovementGrid->mCurrentUnits.size(), category, UnitDataTable.at( unitType ), mTextures, mFonts ) );
    //sf::Rect<float> object = mMovementGrid->mData[gridIndex.y * (WINDOW_WIDTH / TILE_SIZE) + gridIndex.x].mBounds;
    unit->setPosition( gridIndex.x * TILE_SIZE, gridIndex.y * TILE_SIZE );
    unit->mSprite.setTextureRect( UnitDataTable.at( unitType ).textureRect );
    mMovementGrid->addUnit( unit.get( ) );

    //std::cout << "LayerMap is Mis-aligned. ObjectLayer1 should be layer1"
    mSceneLayers.at( LayerMap.at( "ObjectLayer1" ) )->attachChild( std::move( unit ) );
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
                    std::unique_ptr<VertexArrayNode> layer( new VertexArrayNode(  ) );
                    if( !layer.get()->load( mTextures.get( TextureMap.at( map.tileSets[0].name ) ), sf::Vector2u( tileSets.tileWidth, tileSets.tileHeight ), map.layers[i].data, map.width, map.height ) )
                        std::cout << "ERROR loading TiledMap! BuildScene ln: 249" << std::endl;
                    else {
                        mSceneLayers.push_back( layer.get( ) );
                        // init grid
                        std::unique_ptr<Grid> grid( new Grid( this, map.layers[i].width, map.layers[i].height ) );
                        mMovementGrid = grid.get();
                        mMovementGrid->buildGrid( tileSets, map.layers[i] );
                        mSceneGraph.attachChild( std::move( layer ) );
                        mSceneGraph.attachChild( std::move( grid ) );


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

                     if( object.type == "SpawnPoint" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedBuilding;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueBuilding;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Building> sp( new Building( mMovementGrid->mCurrentBuildings.size(), category, BuildingDataTable.at( BuildingTypeMap.at( "SpawnPoint" ) ), mTextures ) );
                         sp->setPosition( object.x - TILE_SIZE / 2, object.y - TILE_SIZE / 2 );
                         mMovementGrid->addBuilding( sp.get() );
                         node.get( )->attachChild( std::move( sp ) );
                     }
                     else if( object.type == "Fighter" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedUnit;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueUnit;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Unit> unit( new Unit( mMovementGrid->mCurrentUnits.size(), category, UnitDataTable.at( UnitTypeMap.at( "Fighter" ) ), mTextures, mFonts ) );
                         unit->setPosition( object.x, object.y - TILE_SIZE - (TILE_SIZE * .5) );
                         unit->mSprite.setTextureRect( UnitDataTable.at( UnitTypeMap.at( "Fighter" ) ).textureRect );
                         mMovementGrid->addUnit( unit.get() );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else if( object.type == "Templar" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedUnit;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueUnit;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Unit> unit( new Unit( mMovementGrid->mCurrentUnits.size(), category, UnitDataTable.at( UnitTypeMap.at( "Templar" ) ), mTextures, mFonts ) );
                         unit->setPosition( object.x, object.y - TILE_SIZE - (TILE_SIZE * .5) );
                         unit->mSprite.setTextureRect( UnitDataTable.at( UnitTypeMap.at( "Templar" ) ).textureRect );
                         mMovementGrid->addUnit( unit.get() );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else if( object.type == "Archer" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedUnit;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueUnit;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Unit> unit( new Unit( mMovementGrid->mCurrentUnits.size(), category, UnitDataTable.at( UnitTypeMap.at( "Archer" ) ), mTextures, mFonts ) );
                         unit->setPosition( object.x, object.y - TILE_SIZE - (TILE_SIZE * .5) );
                         unit->mSprite.setTextureRect( UnitDataTable.at( UnitTypeMap.at( "Archer" ) ).textureRect );
                         mMovementGrid->addUnit( unit.get() );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else if( object.type == "Medic" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedUnit;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueUnit;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Unit> unit( new Unit( mMovementGrid->mCurrentUnits.size(), category, UnitDataTable.at( UnitTypeMap.at( "Medic" ) ), mTextures, mFonts ) );
                         unit->setPosition( object.x, object.y - TILE_SIZE - (TILE_SIZE * .5) );
                         unit->mSprite.setTextureRect( UnitDataTable.at( UnitTypeMap.at( "Medic" ) ).textureRect );
                         mMovementGrid->addUnit( unit.get() );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else if( object.type == "Wizard" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedUnit;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueUnit;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Unit> unit( new Unit( mMovementGrid->mCurrentUnits.size(), category, UnitDataTable.at( UnitTypeMap.at( "Wizard" ) ), mTextures, mFonts ) );
                         unit->setPosition( object.x, object.y - TILE_SIZE - (TILE_SIZE * .5) );
                         unit->mSprite.setTextureRect( UnitDataTable.at( UnitTypeMap.at( "Wizard" ) ).textureRect );
                         mMovementGrid->addUnit( unit.get() );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else if( object.type == "Rogue" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedUnit;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueUnit;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Unit> unit( new Unit( mMovementGrid->mCurrentUnits.size(), category, UnitDataTable.at( UnitTypeMap.at( "Rogue" ) ), mTextures, mFonts ) );
                         unit->setPosition( object.x, object.y - TILE_SIZE - (TILE_SIZE * .5) );
                         unit->mSprite.setTextureRect( UnitDataTable.at( UnitTypeMap.at( "Rogue" ) ).textureRect );
                         mMovementGrid->addUnit( unit.get() );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else if( object.type == "Scout" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedUnit;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueUnit;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Unit> unit( new Unit( mMovementGrid->mCurrentUnits.size(), category, UnitDataTable.at( UnitTypeMap.at( "Scout" ) ), mTextures, mFonts ) );
                         unit->setPosition( object.x, object.y - TILE_SIZE - (TILE_SIZE * .5) );
                         unit->mSprite.setTextureRect( UnitDataTable.at( UnitTypeMap.at( "Scout" ) ).textureRect );
                         mMovementGrid->addUnit( unit.get() );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else if( object.type == "Bard" )
                     {
                         Category::Type category = Category::None;
                         if( object.properties.at( "Team" ) == "Red" ) category = Category::RedUnit;
                         else if( object.properties.at( "Team" ) == "Blue" ) category = Category::BlueUnit;
                         else std::cout << "ERROR reading unit Team/Category! check buildScene/Tiled map save file." << std::endl;

                         std::unique_ptr<Unit> unit( new Unit( mMovementGrid->mCurrentUnits.size(), category, UnitDataTable.at( UnitTypeMap.at( "Bard" ) ), mTextures, mFonts ) );
                         unit->setPosition( object.x, object.y - TILE_SIZE - (TILE_SIZE * .5) );
                         unit->mSprite.setTextureRect( UnitDataTable.at( UnitTypeMap.at( "Bard" ) ).textureRect );
                         mMovementGrid->addUnit( unit.get() );
                         node.get( )->attachChild( std::move( unit ) );
                     }
                     else if( object.type == "TacticalLocation" )
                     {
                         // set value of importance property to the tile found at the same location as this object
                         mMovementGrid->mData[object.y/TILE_SIZE * mMovementGrid->mGridWidth + object.x/TILE_SIZE].importance = std::atof( object.properties.at( "Importance" ).c_str( ) );
                         mMovementGrid->mData[object.y/TILE_SIZE * mMovementGrid->mGridWidth + object.x/TILE_SIZE].debugText->setString( object.properties.at( "Importance" ).c_str( ) )  ;
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
        mSceneGraph.attachChild( std::move( mBlueTeam ) );
        std::unique_ptr<TeamStats> mRedTeam( new TeamStats( Category::TeamRed, mFonts ) );
        mRedTeamStats = mRedTeam.get();
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

void World::loadSaveFile( std::string )
{

}

void World::registerStates( )
{
    mStateStack->registerState<EndTurnMenuState>( States::EndTurnMenuState, this );
    mStateStack->registerState<SpawnPointMenuState>( States::SpawnPointMenuState, this );
    mStateStack->registerState<ActionMenuState>( States::ActionMenuState, this );
    mStateStack->registerState<AbilitySelectMenuState>( States::AbilitySelectMenuState, this );
    mStateStack->registerState<RotationSelectMenuState>( States::RotationSelectMenuState, this );
    mStateStack->registerState<WaitMenu>( States::WaitMenuState, this );
}
