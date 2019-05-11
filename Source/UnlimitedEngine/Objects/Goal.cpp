#include "Goal.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include "Core/Utility.hpp"

#include "Core/Globals.hpp"

Goal::Goal( Owner owner )
    : SceneNode( )
    , mOwner( owner )
    , mSprite( sf::RectangleShape( sf::Vector2f( 200.0f, 30.0f ) ) )
{
    centerOrigin( mSprite );
    mSprite.setOutlineThickness( 2 );
    mSprite.setOutlineColor( sf::Color::White );
    if( mOwner == Owner::TeamA )
    {
        mSprite.setFillColor( sf::Color( 0, 0, 255, 100 ) );
        this->setPosition( WINDOW_WIDTH / 2, WINDOW_HEIGHT - 20 );
    }
    else {
        mSprite.setFillColor( sf::Color( 255, 0, 0, 100 ) );
        this->setPosition( WINDOW_WIDTH / 2, 20 );
    }
}

unsigned int  Goal::getCategory( ) const
{
    if( mOwner == Owner::TeamA )
        return Category::GoalA;
    return Category::GoalB;
}

sf::FloatRect Goal::getBoundingRect( ) const
{
    return getWorldTransform( ).transformRect( mSprite.getGlobalBounds( ) );
}

bool Goal::isMarkedForRemoval( ) const
{
    return false;
}

bool Goal::isDestroyed( ) const
{
    return false;
}

void Goal::updateCurrent( sf::Time, CommandQueue& )
{

}

void Goal::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}
