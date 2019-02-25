#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP


#include "Core/ResourceManager.hpp"
#include "Core/ResourceIdentifiers.hpp"

#include "Core/SceneNode.hpp"
#include "Core/SpriteNode.hpp"
#include "Core/VertexArrayNode.hpp"
#include "Core/CommandQueue.hpp"
#include "Core/Command.hpp"
#include "Core/State.hpp"
#include "Core/PostEffect.hpp"
#include "Core/BloomEffect.hpp"
#include "Core/Globals.hpp"
#include "Core/Player.hpp"
#include "Core/CollisionMan.hpp"

#include "Objects/Actor.hpp"
#include "Objects/Item.hpp"
#include "Objects/Warp.hpp"
#include "Objects/Wall.hpp"

#include "Gui/MessageBoxNode.hpp"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>

// Forward declaration
namespace sf
{
    class RenderTarget;
}

class GameState : public State
{
public:
    GameState( States::ID id, StateStack& stack, Context context );

    ~GameState( );
    virtual void draw( );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

    CommandQueue& getCommandQueue( void );
    bool matchesCategories( std::pair<SceneNode*, SceneNode*>& colliders, Category::Type type1, Category::Type type2 );

    void handleCollisions( void );
    void buildScene( void );

protected:
    sf::RenderTarget&					mTarget;
    sf::RenderTexture					mSceneTexture;
    sf::View							mWorldView;
    TextureManager&						mTextures;
    FontManager&						mFonts;
    MusicPlayer&                        mMusic;
    SoundPlayer&						mSounds;
    State::Context                      mContext;
    sf::Sprite                          mWindowSprite;
    CommandQueue						mCommandQueue;
    BloomEffect							mBloomEffect;
    SceneNode							mSceneGraph;
    std::vector<SceneNode*>          	mSceneLayers;
    Player&                             mPlayer;
    Actor*                              mRed;
};

#endif // GAMESTATE_HPP
