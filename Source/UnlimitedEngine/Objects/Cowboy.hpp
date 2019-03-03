#ifndef COWBOY_HPP
#define COWBOY_HPP

#include <math.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "Objects/Actor.hpp"
#include "Core/ResourceIdentifiers.hpp"
#include "Core/TextNode.hpp"
#include "Core/Entity.hpp"
#include "Core/DataTables.hpp"

#include "Core/Animation.hpp"
#include "Core/EmitterNode.hpp"
#include "Core/SoundNode.hpp"
#include "Core/SoundPlayer.hpp"
#include "Core/SpriteNode.hpp"
#include "Core/Utility.hpp"

#include "Tiled/TiledManager.hpp"



class Cowboy : public Actor
{
    enum Limbs {
          LeftArm
        , RightArm
        , LeftLeg
        , RightLeg
        , LimbCount
    };

    enum Animations {
          Standing
        , WalkLeft
        , WalkRight
        , WalkUp
        , WalkDown
        , DrawGunLeft
        , DrawGunRight
        , Jump
        , TipHat
        , AnimationCount
    };

    struct AnimationFrame {
        AnimationFrame( std::pair<float, float> leftArm, std::pair<float, float> rightArm, std::pair<float, float> leftLeg, std::pair<float, float> rightLeg, std::pair<float, float> sprite )
            :   leftArm( leftArm )
              , rightArm( rightArm )
              , leftLeg( leftLeg )
              , rightLeg( rightLeg )
              , sprite( sprite )
        {
        }

        std::pair<float, float> leftArm;
        std::pair<float, float> rightArm;
        std::pair<float, float> leftLeg;
        std::pair<float, float> rightLeg;
        std::pair<float, float> sprite;
    };

public:
    Cowboy( Tiled::Object data, unsigned int texID, sf::Rect<int> texRect, const TextureManager& textures, SoundPlayer* soundPlayer, const FontManager& fonts );
    virtual bool 		  isMarkedForRemoval( ) const;
    virtual bool          isDestroyed( void ) const;

    virtual void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
    virtual void updateCurrent( sf::Time dt, CommandQueue& commands );
private:
    sf::CircleShape mLeftArm;
    sf::CircleShape mRightArm;

    sf::CircleShape mLeftLeg;
    sf::CircleShape mRightLeg;

    Animations      mCurrentAnimation;
    unsigned int    mCurrentAnimationFrame;
    sf::Time        mMaxFrameTime;
    sf::Time        mCurrentFrameTime;

    std::map<Animations, std::vector<AnimationFrame>> mAnimations;
};

#endif // COWBOY_HPP
