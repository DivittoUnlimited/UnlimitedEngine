#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "Core/ResourceManager.hpp"
#include "Core/ResourceIdentifiers.hpp"
#include "Core/NetworkNode.hpp"
#include "Core/State.hpp"
#include "Core/CommandQueue.hpp"
#include "Core/StateStack.hpp"
#include "Tiled/TiledManager.hpp"
#include "Game/TeamStats.hpp"
#include "Game/DataTables.hpp"
#include "Game/Unit.hpp"
#include "Gui/MessageBoxNode.hpp"
#include "Graphics/BloomEffect.hpp"
#include "Graphics/VertexArrayNode.hpp"
#include "Graphics/SpriteNode.hpp"
#include "Core/StateIdentifiers.hpp"
#include "States/SpawnPointMenuState.hpp"
#include "States/AbilitySelectMenuState.hpp"
#include "States/RotationSelectMenuState.hpp"
#include "States/WaitMenu.hpp"
#include "Game/WifeBot.hpp"

enum InfluenceType
{
    Offensive,
    Defensive,
    BuidingInfluence
};

class Square
{
public:
    Square( unsigned int terrainType, sf::Vector2i gridIndex, sf::Vector2f pos, float size, bool isOccupied = false )
        : terrainType( terrainType )
        , gridIndex( gridIndex )
        , isOccupied( isOccupied )
        , isPossibleNewLocation( false )
        , isPossibleAttackPosition( false )
        , mBounds( pos, sf::Vector2f( size, size ) )
        , unitID( "ERROR" )
        , buildingID( -1 )
        , offensiveInfluence( 0.0f )
        , defensiveInfluence( 0.0f )
        , tacticalInfluence( 0.0f )
        , importance( 0.0f )
        , isVisible( false )
        , debugText( nullptr )
        , costSoFar( 0.0f )
        , heuristicCost( 0.0f )
        , parent( nullptr )
    {
    }

    bool operator==( const Square* other ) { return this->gridIndex == other->gridIndex; }
    bool operator<( const Square* other ) { return( this->costSoFar + this->heuristicCost < other->costSoFar + other->heuristicCost ); }

    /// \brief terrainType
    ///
    unsigned int terrainType;
    /// \brief gridIndex
    ///
    sf::Vector2i gridIndex;
    /// \brief isOccupied
    ///
    bool isOccupied;
    /// \brief isPossibleNewLocation
    ///
    bool isPossibleNewLocation;
    /// \brief isPossibleAttackPosition
    ///
    bool isPossibleAttackPosition;
    /// \brief mBounds
    ///
    sf::Rect<float> mBounds;
    /// \brief unitID
    ///
    std::string unitID;
    /// \brief buildingID
    ///
    int buildingID;
    /// \brief offensiveInfluence
    /// range of -1.0 -> 1.0
    /// positive values represent blue control and vice-versa 0 if unneeded or out of range of both teams
    float offensiveInfluence;
    /// \brief defensiveInfluence
    /// range of -1.0 -> 1.0
    /// positive values represent blue control and vice-versa 0 if unneeded or out of range of both teams
    float defensiveInfluence;
    /// \brief tacticalInfluence
    /// range of -1.0 -> 1.0
    /// positive values represent blue control and vice-versa 0 if unneeded or out of range of both teams
    float tacticalInfluence;
    /// \brief importance
    /// Manually loaded from lua in the buildScene method this is the value the AI should place on this
    /// location
    float importance;
    /// \brief isVisible
    /// used to check if this square should be covered by the fog of war(give the rect below a grey transparent fill color)
    bool isVisible;
    /// \brief rect
    ///
    RectangleShapeNode* rect;
    /// \brief debugText
    ///
    TextNode* debugText;
    // used by WifeBot
    /// \brief costSoFar
    ///
    float costSoFar;
    /// \brief heuristicCost
    ///
    float heuristicCost;
    /// \brief parent
    ///
    Square* parent;
};

/// \brief The World class
///
class World
{
public:
//## Interface
    /// \brief World
    ///
    World( State::Context* context, StateStack* stack, sf::RenderTarget& outputTarget, FontManager& fonts, SoundPlayer& sounds, unsigned int level, bool networked, bool isLocalMultiplayer );

    ~World( void ) { }
    /// \brief handleEvent
    /// \param event
    ///
    void handleEvent( const sf::Event &event );
    /// \brief handleLeftClick
    /// \param mousePos
    ///
    void handleLeftClick( sf::Vector2i mousePos );
    /// \brief update
    /// \param dt
    /// \return
    ///
    bool update( sf::Time dt );
    /// \brief draw
    ///
    void draw( void );
    /// \brief getCommandQueue
    /// \return
    ///
    CommandQueue& getCommandQueue( void );
    /// \brief pollGameAction
    /// \param out
    /// \return
    ///
    bool pollGameAction( GameActions::Action& out );
    /// \brief spawnUnit
    /// \param unitType
    /// \param gridIndex
    ///
    void spawnUnit(Category::Type unitType, sf::Vector2i gridIndex );
    /// \brief getPossiblePositions
    /// \param startingPoint
    /// \param unitType
    /// \param distanceLeft
    /// \return
    ///
    std::vector<sf::Vector2i> getPossiblePositions( sf::Vector2i startingPoint, unsigned int unitType, int distanceLeft );
    /// \brief getPossibleAttackPositions
    /// \param startingPoint
    /// \param innerRange
    /// \param outerRange
    /// \return
    ///
    std::vector<sf::Vector2i> getPossibleAttackPositions( sf::Vector2i startingPoint, int innerRange, int outerRange );
    /// \brief showUnitMovment
    /// Shows the available squares a unit can move to by highlighting them in a diferent color
    /// \param i
    ///
    /// \param j
    ///
    void showUnitMovement( unsigned int i, unsigned int j );
    /// \brief updateFogOfWar
    ///
    void updateFogOfWar( void );
    /// \brief buildScene
    /// \param tileMapFilePath
    ///
    void buildScene( std::string tileMapFilePath );
    /// \brief registerStates
    /// Register all possible states for later use
    void registerStates( );
    /// \brief Grid::addUnit
    /// \param unit
    ///
    void addUnit( Unit* unit );
    /// \brief removeUnit
    /// \param position
    ///
    void removeUnit( sf::Vector2i position );
    /// \brief moveUnit
    /// \param currentPos
    /// \param newPos
    /// \return
    ///
    bool moveUnit( sf::Vector2i currentPos, sf::Vector2i newPos );
    /// \brief getTartgets
    /// \param possibleAttackLocations
    ///
    void getTartgets( std::vector<sf::Vector2i> possibleAttackLocations );
    /// \brief clearGrid
    //
    void clearGrid( void );
    /// \brief buildPossiblePositions
    /// \param mPossiblePositions
    /// \param startingPoint
    /// \param unitType
    /// \param distanceLeft
    void buildPossiblePositions( std::vector<sf::Vector2i>* mPossiblePositions, sf::Vector2i startingPoint, unsigned int unitType, int distanceLeft );
    /// \brief getMoveCost
    /// \param unitType
    /// \param terrainType
    /// \return
    ///
    unsigned int getMoveCost( int unitType, unsigned int terrainType );
    /// \brief endTurn
    ///
    void endTurn( void );
    /// \brief checkVictoryConditions
    /// \return
    ///
    bool checkVictoryConditions( void );
//## Attributes
    /// \brief mTarget
    /// The main window of the game.
    sf::RenderTarget&					mTarget;
    /// \brief mSceneTexture
    /// Used to draw the game world to screen
    sf::RenderTexture					mSceneTexture;
    /// \brief mWorldView
    /// The "lens" of the camera the player views the world from
    sf::View							mWorldView;
    /// \brief mTextures
    /// The textures to build sprites and other art assets from
    TextureManager						mTextures;
    /// \brief mFonts
    /// The fonts available to be drawn on screen
    FontManager&						mFonts;
    /// \brief mMusic
    /// Abstraction to control music and other longer audio files
    MusicPlayer*                        mMusic;
    /// \brief mSounds
    /// Abstraction to controll sound effects
    SoundPlayer&						mSounds;
    /// \brief mWindowSprite
    /// The image that will be drawn to the screen.
    sf::Sprite                          mWindowSprite;
    /// \brief mCommandQueue
    /// A list of commands built up over the last update step, must be cleared between updates.
    CommandQueue						mCommandQueue;
    /// \brief mBloomEffect
    /// Abstraction for lighting effect
    BloomEffect							mBloomEffect;
    /// \brief mSceneGraph
    /// The main data structure for handling all things that are drawn to the screen.
    SceneNode							mSceneGraph;
    /// \brief mSceneLayers
    /// The layers that make up the graphical interface of the game. ( tiles, decorations, units, HUD, etc.. )
    std::vector<SceneNode*>          	mSceneLayers;
    /// \brief mDeltaMousePosition
    /// used to track mouse position considering the offset caused by moving the worldView (Camera)
    sf::Vector2f                        mDeltaMousePosition;
    /// \brief mCameraPanSpeed
    /// How fast the camera should move when scolling around in the world
    float                               mCameraPanSpeed;
    /// \brief mNetworkedWorld
    /// Is this game played of a network
    bool								mNetworkedWorld;
    /// \brief mLocalMultiplayerWorld
    /// Is this game a couch co-op game?
    bool                                mLocalMultiplayerWorld;
    /// \brief mNetworkNode
    /// Abstraction for handling network gameplay
    NetworkNode*						mNetworkNode;
    /// \brief mWorldContext
    /// Contains things like fonts, soundEffects, etc..
    State::Context                      mWorldContext;
    /// \brief mStateStack
    /// A list of all the states that are still active. Often times there are more than one state active such as when a menu is popped or when the user enters the pause screen.
    StateStack*                         mStateStack;
    /// \brief mSelectedUnit
    /// Id of the unit currently being manipulated.
    std::string                         mSelectedUnit;
    /// \brief mClientTeamColor
    /// Used in networked games to tell if this world should react to events or not.
    Category::Type                      mClientTeamColor;
    /// \brief mBlueTeamStats
    ///  Container class for all the stats held by the team
    /// as well as the source for the HUD data.
    /// Child of SceneNode, This object must be attached to the SceneGraph
    /// above all other layer to show up on the screen
    TeamStats*                           mBlueTeamStats;
    /// \brief mRedTeamStats
    ///  Container class for all the stats held by the team
    /// as well as the source for the HUD data
    /// Child of SceneNode, This object must be attached to the SceneGraph
    /// above all other layer to show up on the screen
    TeamStats*                           mRedTeamStats;

    ////////// OLD GRID ATTRIBUTES

    /// \brief mGrid
    /// The main data structure for the whole battle has info on unit and building positions as well as the terrain type
    /// of the grid squares, the influence each team has on a square, and how much of a threat a square is to each team.
    std::vector<Square> mGrid;
    /// \brief mGridWidth
    ///
    unsigned int        mGridWidth;
    /// \brief mGridHeight
    ///
    unsigned int        mGridHeight;
    /// \brief mUpdateFogOfWar
    ///
    bool                mUpdateFogOfWar;
    /// \brief mUpdateInfluenceMap
    ///
    bool                mUpdateInfluenceMap;
    /// \brief mUpdateThreatLevelMap
    ///
    bool                mUpdateThreatLevelMap;
    /// \brief mShowInfluenceMap
    ///
    bool                mShowInfluenceMap;
    /// \brief mShowThreatLevelMap
    ///
    bool                mShowThreatLevelMap;
    /// \brief mWaitingForPlayer
    ///
    bool                mWaitingForPlayer;
    /// \brief mSelectedGridIndex
    ///
    sf::Vector2i        mSelectedGridIndex;
    /// \brief mEndTurn
    ///
    bool                mEndTurn;
    /// \brief mCurrentUnits
    /// A list of all units currently alive on the battle field.
    std::vector<Unit*>  mCurrentUnits;
    /// \brief mWifeBot
    ///
    WifeBot*         mWifeBot;
};

#endif // WORLD_HPP
