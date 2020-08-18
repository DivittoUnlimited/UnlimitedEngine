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
#include "Core/PerlinNoise.hpp"
#include "Core/StateIdentifiers.hpp"
#include "Core/Globals.hpp"
#include "Core/Utility.hpp"
#include "Tiled/TiledManager.hpp"
#include "Gui/MessageBoxNode.hpp"
#include "Graphics/BloomEffect.hpp"
#include "Graphics/VertexArrayNode.hpp"
#include "Graphics/SpriteNode.hpp"
#include "Game/DataTables.hpp"


/// \brief The World class
///
class World
{
public:
//## Interface
    /// \brief World
    ///
    World(State::Context* context, StateStack* stack, sf::RenderTarget& outputTarget, FontManager& fonts, SoundPlayer& sounds, bool networked, bool isLocalMultiplayer );

    ~World( void ) { }
    /// \brief handleEvent
    /// \param event
    ///
    void handleEvent( const sf::Event &event );
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
    /// \brief buildScene
    /// \param tileMapFilePath
    ///
    void buildScene();
    /// \brief generateTileMap
    /// \param width
    /// \param height
    /// \param increment
    /// \return
    ///
    std::vector<unsigned int> generateTileMap( unsigned int width, unsigned int height, double increment );
    /// \brief registerStates
    /// Register all possible states for later use
    void registerStates( );

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

    /// GAME ATTRIBUTES
    /// \brief mPlayerAvatar
    /// The sprite that represents the player on screen
    SpriteNode*                         mPlayerAvatar;



};

#endif // WORLD_HPP
