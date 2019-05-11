#ifndef STARSHIP_HPP
#define STARSHIP_HPP

#include <SFML/Graphics/ConvexShape.hpp>

#include "Core/Entity.hpp"

class StarShip : public Entity
{
public:
    StarShip( bool isPlayer = false );

    virtual unsigned int  getCategory( ) const;
    virtual sf::FloatRect getBoundingRect( ) const;
    virtual bool 		  isMarkedForRemoval( ) const;
    virtual void          updateCurrent( sf::Time dt, CommandQueue& commands );
    void                  drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;

    float                 speed( void ) const { return mSpeed; }
    void                  speed( float speed ) { this->mSpeed = speed; }
    float                 maximumVelocity( void ) const { return mMaxVelocity; }
    void                  maximumVelocity( float maxVel ) { this->mMaxVelocity = maxVel; }
    bool                  getHasFlag( void ) const { return this->mHasFlag; }
    void                  setHasFlag( bool hasFlag ) { this->mHasFlag = hasFlag; }

private:
    sf::ConvexShape mSprite;
    bool mIsPlayer;
    float mSpeed;
    float mMaxVelocity;
    bool mHasFlag;

};

#endif // STARSHIP_HPP
