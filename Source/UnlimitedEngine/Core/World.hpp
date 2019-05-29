#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Objects/StarShip.hpp"
#include "Objects/Flag.hpp"
#include "Objects/Goal.hpp"
#include "Objects/Bumper.hpp"
#include "Gui/MessageBoxNode.hpp"

#include "Core/ResourceManager.hpp"
#include "Core/ResourceIdentifiers.hpp"

#include "Core/SceneNode.hpp"
#include "Core/SpriteNode.hpp"

#include "Core/CommandQueue.hpp"
#include "Core/Command.hpp"
#include "Core/State.hpp"
#include "Core/PostEffect.hpp"
#include "Core/BloomEffect.hpp"
#include "Core/Globals.hpp"
#include "Core/Player.hpp"
#include "Core/CollisionMan.hpp"
#include "Core/EmitterNode.hpp"
#include "Core/NetworkNode.hpp"

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
    World( sf::RenderTarget& outputTarget, FontManager& fonts, SoundPlayer& sounds, bool networked = false, bool isLocalMultiplayer = false );
    ~World( void );
    void draw( void );
    bool update( sf::Time dt );
    CommandQueue& getCommandQueue( void );
    StarShip* getStarShip( int identifier ) const;
    StarShip* addStarShip(int identifier , Category::Type category);
    void removeStarShip( int identifier );
    bool pollGameAction( GameActions::Action& out );
    bool matchesCategories( std::pair<SceneNode*, SceneNode*>& colliders, Category::Type type1, Category::Type type2 );

    void handleCollisions( void );

    void buildScene( void );

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
    unsigned int                        mTeamAScore;
    unsigned int                        mTeamBScore;
};

#endif // WORLD_HPP
