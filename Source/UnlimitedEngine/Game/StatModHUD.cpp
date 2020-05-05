#include "StatModHUD.hpp"


StatModHUD::StatModHUD( const FontManager& fonts, std::string text, sf::Vector2i pos )
    : mTimer( sf::milliseconds( 3000 ) )
    , mText( sf::Text( text, fonts.get( FontMap.at( "Default" ) ), 18 ) )
    , mStartX( pos.x )
    , mStartY( pos.y )
{
    mText.setFillColor( sf::Color::White );
    mText.setPosition( pos.x, pos.y );
}

unsigned int StatModHUD::getCategory( ) const
{
    return Category::None;
}

bool StatModHUD::isMarkedForRemoval( ) const
{
    return false;
}

bool StatModHUD::isDestroyed( ) const
{
    return false;
}

void StatModHUD::updateCurrent( sf::Time dt, CommandQueue& )
{
    mText.move( 0, -2 * dt.asSeconds( ) );
    if( mTimer != sf::Time::Zero ) mTimer -= dt;
    else if( mTimer > sf::milliseconds( 6000 ) ) mTimer = sf::milliseconds( 6000 );

    if( mTimer < sf::Time::Zero )
    {
        mText.setString( "" );
        mText.setPosition( mStartX, mStartY );
        mTimer = sf::Time::Zero;
    }
}

void StatModHUD::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mText, states );
}
