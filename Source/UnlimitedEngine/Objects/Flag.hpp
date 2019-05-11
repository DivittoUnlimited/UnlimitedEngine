#ifndef FLAG_HPP
#define FLAG_HPP

#include <SFML/Graphics/ConvexShape.hpp>
#include "Core/SceneNode.hpp"
#include "Objects/StarShip.hpp"

class Flag : public SceneNode
{
public:
    Flag( bool isTeamAFlag, sf::Vector2f startingPos );

    virtual unsigned int  getCategory( ) const;
    virtual sf::FloatRect getBoundingRect( ) const;
    virtual bool 		  isMarkedForRemoval( ) const;
    virtual bool          isDestroyed( ) const;
    virtual void          updateCurrent( sf::Time dt, CommandQueue& commands );
    void                  drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
    void                  setStarShip( StarShip* starShip ) { this->mStarShip = starShip; }
    StarShip*             getStarShip( void )  { return this->mStarShip; }
    void setFillColor( sf::Color color ) { this->mSprite.setFillColor( color ); }
    const sf::Vector2f    getStartingPos( void ) const { return mStartingPos; }

private:
    sf::ConvexShape mSprite;
    bool            misTeamAFlag;
    StarShip*       mStarShip;
    sf::Vector2f    mStartingPos;
};

#endif // FLAG_HPP
