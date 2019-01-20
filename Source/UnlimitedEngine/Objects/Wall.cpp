#include "Wall.hpp"
#include "Core/Utility.hpp"

Wall::Wall( Tiled::Object data, sf::RectangleShape sprite )
: mType( data.type )
, mSprite( sprite )
{
   // mSprite.setOrigin( sf::Vector2f( mSprite.getSize().x / 2, mSprite.getSize( ).y / 2 ) ); // the center
    // Build object using resource manager and data from tiled map
    this->setPosition( data.x, data.y );
    mSprite.setFillColor( sf::Color( 255, 0, 0, 0 ) );
}

unsigned int Wall::getCategory( ) const
{
    return Category::Wall;
}

sf::FloatRect Wall::getBoundingRect( ) const
{
    return getWorldTransform( ).transformRect( mSprite.getGlobalBounds( ) );
}

bool Wall::isMarkedForRemoval( ) const
{
    return false;
}

void Wall::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}

void Wall::updateCurrent( sf::Time, CommandQueue& )
{
}
