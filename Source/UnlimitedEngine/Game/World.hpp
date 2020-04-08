#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>


#include "Gui/MessageBoxNode.hpp"

#include "Core/ResourceManager.hpp"
#include "Core/ResourceIdentifiers.hpp"
#include "Game/DataTables.hpp"

#include "Graphics/SceneNode.hpp"
#include "Graphics/SpriteNode.hpp"
#include "Graphics/RectangleShapeNode.hpp"

#include "Core/CommandQueue.hpp"
#include "Core/Command.hpp"
#include "Core/State.hpp"
#include "Graphics/PostEffect.hpp"
#include "Graphics/BloomEffect.hpp"
#include "Graphics/TextNode.hpp"
#include "Core/Globals.hpp"
#include "Core/Player.hpp"
#include "Core/CollisionMan.hpp"
#include "Graphics/EmitterNode.hpp"
#include "Core/NetworkNode.hpp"
#include "Core/StateStack.hpp"
#include "Game/Grid.hpp"
#include "Game/Unit.hpp"
#include "Game/Building.hpp"

enum Ships {
      TeamAShip1 = 0
    //, TeamAShip2
    //, TeamAShip3
    , TeamBShip1
    //, TeamBShip2
    //, TeamBShip3
    , Count
};

class World
{
public:
    World( State::Context *context, StateStack *stack, sf::RenderTarget& outputTarget, FontManager& fonts, SoundPlayer& sounds, unsigned int level, bool networked = true, bool isLocalMultiplayer = true );
    ~World( void );
    void draw( void );
    bool update( sf::Time dt );
    CommandQueue& getCommandQueue( void );
    bool pollGameAction( GameActions::Action& out );
    bool matchesCategories( std::pair<SceneNode*, SceneNode*>& colliders, Category::Type type1, Category::Type type2 );

    void handleEvent( const sf::Event &event );
    void handleCollisions( void );

    void changeTurn( void );
    void spawnUnit( unsigned int unitType, sf::Vector2i gridIndex );

    void buildScene( std::string tileMapFilePath );
    void loadSaveFile( std::string saveFilePath );
    ///
    /// \brief registerStates
    /// Register all possible states for later use
    void registerStates( );

    int getSelectedUnit( void ) { return mSelectedUnit; }
    void setSelectedUnit( int id ) { this->mSelectedUnit = id; }

    void setSelectedBuilding( int selectedBuilding ) { this->mSelectedBuilding = selectedBuilding; }
    int getSelectedBuilding( void ) { return mSelectedBuilding; }

private:
    sf::RenderTarget&					mTarget;
    sf::RenderTexture					mSceneTexture;
    sf::View							mWorldView;
    TextureManager						mTextures;
public:
    FontManager&						mFonts;  // made private becuase grid needed it and i am lazy
private:
    MusicPlayer*                        mMusic;
    SoundPlayer&						mSounds;
    sf::Sprite                          mWindowSprite;
    CommandQueue						mCommandQueue;
    BloomEffect							mBloomEffect;
public:
    SceneNode							mSceneGraph;
private:
    std::vector<SceneNode*>          	mSceneLayers;
public:
    /// \brief mDeltaMousePosition
    /// used to track mouse position considering the offset caused by moving the worldView (Camera)
    sf::Vector2f                        mDeltaMousePosition;
    float                               mCameraPanSpeed;
    bool								mNetworkedWorld;
    bool                                mLocalMultiplayerWorld;

    NetworkNode*						mNetworkNode;

    // public so grid can access, consider making grid a friend class
    // Friends can touch your privates!!! :p
    State::Context                      mWorldContext;
    StateStack*                         mStateStack;

    // Game specific
    ///
    /// \brief mMovementGrid
    /// The main data structure that houses all the information about the current battlefield
    Grid*                                mMovementGrid;

    ///
    /// \brief mLevel
    /// id for the current level map being loaded, used with the LevelMap table found in DataTables.hpp
    unsigned int mLevel;

    int mSelectedUnit;

    ///
    /// \brief mSelectBuilding
    /// used to track which building was used to pop a menu or something
    /// returns int to represent the id of the building or -1 if no building is selected. (ints index in thhe currentBuildings vector)
    int mSelectedBuilding;

    ///
    /// \brief mCurrentTurn
    /// The currently active team. (RED or BLUE)
    Category::Type                      mCurrentTurn;

    ///
    /// \brief mClientTeamColor
    /// Used in networked games to tell if this world should react to events or not.
    Category::Type                      mClientTeamColor;

    ///
    /// \brief mLoadUnitsFromFile
    /// Flag to tell build scene to load from Tiled map or a player's save file
    bool                                mLoadUnitsFromFile;

    ///
    /// \brief mChangeTurnText
    /// Text that pops up on screen to tell the players that the turn has changed
    sf::Text                            mChangeTurnText;

    ///
    /// \brief mChangeTurnTextTimer
    /// The length of time that the change turn text should be displayed over the game.
    sf::Time                            mChangeTurnTextTimer;

};

#endif // WORLD_HPP
