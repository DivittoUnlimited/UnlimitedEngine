#ifndef STARSHIP_HPP
#define STARSHIP_HPP

#include <SFML/Graphics/ConvexShape.hpp>

#include "Core/Entity.hpp"
#include "Core/Command.hpp"
#include "Objects/Bullet.hpp"

class StarShip : public Entity
{
public:
    StarShip( Category::Type category );

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
    void                  fire( void );
    void                  thrust( void );
    int                   getIdentifier( void ) { return mIdentifier; }
    void                  setIdentifier( int identifier ) { mIdentifier = identifier; }
    void                  setCategory( Category::Type cat );
    sf::ConvexShape*      getSprite( void ) { return &mSprite; }

private:
    int mIdentifier;
    sf::ConvexShape mSprite;
    float mSpeed;
    float mMaxVelocity;
    bool mHasFlag;
    bool mFire;
    Category::Type mCategory;
    sf::Time       mShotTimer;
    sf::Time       mReloadTime;
    Command        mFireCommand;
};

#endif // STARSHIP_HPP
