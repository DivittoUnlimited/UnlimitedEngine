#include "Trigger.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Trigger::Trigger( Tiled::Object data, sf::RectangleShape sprite )
: mType( data.type )
, mSprite( sprite )
{
   // mSprite.setOrigin( sf::Vector2f( mSprite.getSize().x / 2, mSprite.getSize( ).y / 2 ) ); // the center
    // Build object using resource manager and data from tiled map
    this->setPosition( data.x, data.y );
    mSprite.setFillColor( sf::Color( 0, 0, 255, 200 ) );
}

unsigned int Trigger::getCategory( ) const
{
    return Category::Trigger;
}

sf::FloatRect Trigger::getBoundingRect( ) const
{
    return getWorldTransform( ).transformRect( mSprite.getGlobalBounds( ) );
}

bool Trigger::isMarkedForRemoval( ) const
{
    return false;
}

void Trigger::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}

void Trigger::updateCurrent( sf::Time, CommandQueue& )
{
}
