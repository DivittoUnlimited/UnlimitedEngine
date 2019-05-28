#ifndef BUMPER_HPP
#define BUMPER_HPP

#include <SFML/Graphics/ConvexShape.hpp>
#include "Core/Category.hpp"
#include "Core/Entity.hpp"

#include "Core/Globals.hpp"

class Bumper : public Entity
{
public:
    Bumper( Category::Type category );

    virtual unsigned int  getCategory( ) const;
    virtual sf::FloatRect getBoundingRect( ) const;
    virtual bool 		  isMarkedForRemoval( ) const;
    virtual void          updateCurrent( sf::Time dt, CommandQueue& commands );
    void                  drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
    virtual bool    isDestroyed( ) const;

    float                 speed( void ) const { return mSpeed; }
    void                  speed( float speed ) { this->mSpeed = speed; }
    float                 maximumVelocity( void ) const { return mMaxVelocity; }
    void                  maximumVelocity( float maxVel ) { this->mMaxVelocity = maxVel; }
private:
    sf::ConvexShape mSprite;
    Category::Type  mCategory;

    float           mSpeed;
    float           mMaxVelocity;

};

#endif // BUMPER_HPP
