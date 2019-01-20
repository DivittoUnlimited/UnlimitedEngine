#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <math.h>

#include <SFML/Graphics/Sprite.hpp>

#include "Core/ResourceIdentifiers.hpp"
#include "Core/TextNode.hpp"
#include "Core/Entity.hpp"
#include "Core/DataTables.hpp"

#include "Core/Animation.hpp"
#include "Core/EmitterNode.hpp"
#include "Core/SoundNode.hpp"
#include "Core/SoundPlayer.hpp"

#include "Tiled/TiledManager.hpp"


class Actor : public Entity
{
public:
    Actor( Tiled::Object data, sf::Sprite sprite, const TextureManager& textures, SoundPlayer* soundPlayer, const FontManager& fonts );

    virtual unsigned int  getCategory( ) const;
    virtual sf::FloatRect getBoundingRect( ) const;
    virtual bool 		  isMarkedForRemoval( ) const;

    virtual void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
    virtual void updateCurrent( sf::Time dt, CommandQueue& commands );

    void updateMovementPattern( sf::Time dt );

    std::string type( void ) { return mType; }
    float speed( void ) { return mSpeed; }
    void speed( float speed ) { this->mSpeed = speed; }

private:
    sf::Sprite      mSprite;
    std::string     mType;
    std::string     mName;
    float           mTravelledDistance;
    std::size_t     mDirectionIndex;
    TextNode*       mLabel;
    float           mSpeed;
};

#endif // ACTOR_HPP
