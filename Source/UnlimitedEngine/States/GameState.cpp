#include "GameState.hpp"
#include "Core/MusicPlayer.hpp"
#include "Core/Globals.hpp"

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
{
    mWorldView.zoom( .70 );
    if( !mSceneTexture.create( mTarget.getView().getSize().x, mTarget.getSize( ).y ) ) std::cout << "Render ERROR" << std::endl;
    mSceneTexture.setView( mWorldView );
    buildScene( );
    mWorldView.setCenter( mRed->getPosition() );
}

GameState::~GameState( )
{
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
        mBloomEffect.apply( mSceneTexture, mTarget ); // When u re-add bloom effects remove the window sprite scale in the cinstructor for some reason bloom effect messes with that
    }
    else
    {
        mTarget.setView( mWorldView );
        mTarget.draw( mSceneGraph );
    }
}

bool GameState::update( sf::Time dt )
{
    mWorldView.setCenter( mRed->getPosition() );
    while( !mCommandQueue.isEmpty( ) )
        mSceneGraph.onCommand( mCommandQueue.pop( ), dt );
    mSceneGraph.update( dt, mCommandQueue );
    handleCollisions( );

    mPlayer.handleRealtimeInput( mCommandQueue );

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
    CollisionMan::QUAD_TREE.setRootRect( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT ); // mod this to move with player!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    std::set<std::pair<SceneNode*, SceneNode*>> collisionPairs = CollisionMan::update( mSceneLayers[LayerMap.at( "ObjectLayer" )] );

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
            auto& player = static_cast<Actor&>( *pair.first );
            auto& npc = static_cast<Actor&>( *pair.second );

            if( npc.speak( ) ) /// This still needs a LOT of love but im trying to fix QuadTree first, this works as decent  test for that for now
            {
                npc.speak( false );
                npc.setVelocity( 0.0f, 0.0f );
                player.setVelocity( 0.0f, 0.0f );
                requestStackPush( States::MessageBox );
            }
        }
        else {
            std::cout << "Error occured while checking Collision something is being checked that shouldn't be!" << std::endl;
        }
    }
}

void GameState::buildScene( )
{
    ///
    ///
    /// Where does the level init happen??
    ///     - refering to when there are more then one level and not just a single "world"
    ///     - See flow chart on your phone.

    // load TiledMap
    std::cout << "World::buildScene using HARCODED filepath to load Tiled map untill levels can be loaded properly instead of using Game.lua for everything. TiledMaps get loaded from level files" << mContext.tiledMapFilePath << std::endl;
    Tiled::TiledMap map = Tiled::loadFromFile( "Game/Levels/Greenville.lua" ); //mContext.TiledMapFilePath );

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
        if( map.layers[i].type == "tilelayer" )
        {
            auto tileSets = map.tileSets[0];
            if( map.layers[i].name == "TileLayer3" )
            {
                std::unique_ptr<VertexArrayNode> layer( new VertexArrayNode( Category::ParticleLayer ) );
                if( !layer.get()->load( mTextures.get( TextureMap.at( tileSets.name ) ), sf::Vector2u( tileSets.tileWidth, tileSets.tileHeight ), map.layers[i].data , map.width, map.height ) )
                    std::cout << "ERROR loading TiledMap! BuildScene ln: 249" << std::endl;
                else {
                    mSceneLayers.push_back( layer.get( ) );
                    mSceneGraph.attachChild( std::move( layer ) );
                }
            }
            else
            {
                std::unique_ptr<VertexArrayNode> layer( new VertexArrayNode( Category::TileLayer ) );
                if( !layer.get()->load( mTextures.get( TextureMap.at( tileSets.name ) ), sf::Vector2u( tileSets.tileWidth, tileSets.tileHeight ), map.layers[i].data , map.width, map.height ) )
                    std::cout << "ERROR loading TiledMap! BuildScene ln: 249" << std::endl;
                else {
                    mSceneLayers.push_back( layer.get( ) );
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
                 if( object.type == "Player" )
                 {
                     std::unique_ptr<Actor> actor( new Actor( object, TextureMap.at( tiles[object.gid].texID ), tiles[object.gid].rect, mTextures, &mSounds, mFonts ) );
                     this->mRed = actor.get( );
                     node.get( )->attachChild( std::move( actor ) );
                 }
                 else if( object.type == "Wall" )
                 {
                     std::unique_ptr<Wall> wall( new Wall( object, sf::RectangleShape( sf::Vector2f( object.width, object.height ) ) ) );
                     node.get( )->attachChild( std::move( wall ) );
                 }else if( object.type == "Warp" )
                 {
                     std::unique_ptr<Warp> warp( new Warp( object, sf::RectangleShape( sf::Vector2f( object.width, object.height ) ), mTextures, mFonts ) );
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

}

