#ifndef BULLET_HPP
#define BULLET_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "SFML/System/Time.hpp"

#include "Core/Entity.hpp"


class Bullet : public Entity
{
public:
    Bullet(sf::Vector2f pos, float angle );

    virtual unsigned int  getCategory( ) const;
    virtual sf::FloatRect getBoundingRect( ) const;
    virtual bool 		  isMarkedForRemoval( ) const;
    virtual bool          isDestroyed( ) const;
    virtual void          updateCurrent( sf::Time dt, CommandQueue& commands );
    void                  drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;

    ///
    /// \brief getDamageOutput
    /// damage is equal to half the percentage of life the bullet has remaining. i.e 60% of life left == 30 damaage
    /// \return
    ///
    float                 getDamageOutput( void ) { return .5f * (100.0f - (mLifeSpan.asMilliseconds( ) * 100.0f) / mStartingLifeSpan.asMilliseconds( ) ); }
    float                 maximumVelocity( void ) const { return mMaxVelocity; }
    void                  maximumVelocity( float maxVel ) { this->mMaxVelocity = maxVel; }

private:
    sf::Time mStartingLifeSpan;
    sf::Time mLifeSpan;
    sf::CircleShape mSprite;
    float mMaxVelocity;
    float mForce;

};

#endif // BULLET_HPP
