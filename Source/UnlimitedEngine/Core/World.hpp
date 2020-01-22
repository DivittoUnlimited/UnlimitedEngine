#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>


#include "Gui/MessageBoxNode.hpp"

#include "Core/ResourceManager.hpp"
#include "Core/ResourceIdentifiers.hpp"
#include "Core/DataTables.hpp"

#include "Graphics/SceneNode.hpp"
#include "Graphics/SpriteNode.hpp"
#include "Graphics/RectangleShapeNode.hpp"

#include "Core/CommandQueue.hpp"
#include "Core/Command.hpp"
#include "Core/State.hpp"
#include "Graphics/PostEffect.hpp"
#include "Graphics/BloomEffect.hpp"
#include "Core/Globals.hpp"
#include "Core/Player.hpp"
#include "Core/CollisionMan.hpp"
#include "Graphics/EmitterNode.hpp"
#include "Core/NetworkNode.hpp"
#include "GameObjects/Grid.hpp"
#include "GameObjects/Unit.hpp"


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
    World(sf::RenderTarget& outputTarget, FontManager& fonts, SoundPlayer& sounds, unsigned int level, bool networked = false, bool isLocalMultiplayer = false );
    ~World( void );
    void draw( void );
    bool update( sf::Time dt );
    CommandQueue& getCommandQueue( void );
    bool pollGameAction( GameActions::Action& out );
    bool matchesCategories( std::pair<SceneNode*, SceneNode*>& colliders, Category::Type type1, Category::Type type2 );

    void handleEvent( const sf::Event &event );
    void handleCollisions( void );

    void buildScene( std::string tileMapFilePath );
    void loadSaveFile( std::string saveFilePath );


private:
    sf::RenderTarget&					mTarget;
    sf::RenderTexture					mSceneTexture;
    sf::View							mWorldView;
    TextureManager						mTextures;
    FontManager&						mFonts;
    MusicPlayer*                         mMusic;
    SoundPlayer&						mSounds;
    State::Context                      mContext;
    sf::Sprite                          mWindowSprite;
    CommandQueue						mCommandQueue;
    BloomEffect							mBloomEffect;
    SceneNode							mSceneGraph;

    std::vector<SceneNode*>          	mSceneLayers;   
    bool								mNetworkedWorld;
    bool                                mLocalMultiplayerWorld;
    NetworkNode*						mNetworkNode;

    // Game specific
    Grid                                mMovementGrid;
    std::vector<std::vector<RectangleShapeNode*>>    mDrawableGrid;
    unsigned int mLevel; // id for the current level map being loaded
    std::vector<Unit*>                  mCurrentUnits;
    bool                                mLoadUnitsFromFile;
    bool                                mUnitAwaitingOrders;
    sf::Vector2i                        mSelectedGridIndex;
    int                                 mSelectedUnitID;

};

#endif // WORLD_HPP
