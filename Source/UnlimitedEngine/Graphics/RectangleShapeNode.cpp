#include "RectangleShapeNode.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

RectangleShapeNode::RectangleShapeNode( const sf::IntRect& rect )
    : mSprite( sf::Vector2f( rect.width, rect.height ) )
{
    mSprite.setPosition( rect.left, rect.top );
}

void RectangleShapeNode::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}
