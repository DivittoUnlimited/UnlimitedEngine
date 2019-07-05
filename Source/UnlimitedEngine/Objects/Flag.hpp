#ifndef FLAG_HPP
#define FLAG_HPP

#include <SFML/Graphics/ConvexShape.hpp>
#include "Core/EmitterNode.hpp"
#include "Objects/StarShip.hpp"

class Flag : public Entity
{
public:
    Flag( Category::Type category, sf::Vector2f startingPos );

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
    bool                  isCapturable( void ) { return mCapturable; }
    void                  setCapturable( bool capturable ) { this->mCapturable = capturable; }
private:
    sf::ConvexShape mSprite;
    Category::Type  mCategory;
    StarShip*       mStarShip;
    sf::Vector2f    mStartingPos;
    bool            mCapturable;
};

#endif // FLAG_HPP
