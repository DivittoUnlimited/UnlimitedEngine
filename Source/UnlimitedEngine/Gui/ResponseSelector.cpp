#include "ResponseSelector.hpp"

#include "ResponseSelector.hpp"
#include "Core/Utility.hpp"
#include "Core/DataTables.hpp"


#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace GUI
{

ResponseSelector::ResponseSelector( std::string text, const FontManager& fonts )
: mCallback( )
, mNormal(   sf::Color(   0,   0,   0,   0 ) )
, mSelected( sf::Color( 204, 204, 255, 180 ) )
, mPressed(  sf::Color( 204, 204, 255, 200 ) )
, mText( text, fonts.get( FontMap.at( "Default" ) ), 16 )
, mIsToggle( true )
, mSprite( sf::Vector2f( 550, 20 ) )
{
    mText.setFillColor( sf::Color::Black );

    mSprite.setFillColor( mNormal );
    mSprite.setOutlineThickness( 1 );
    mSprite.setOutlineColor( sf::Color( 0, 0, 0, 255 ) );

    mText.setPosition( 10, 0 );
}

void ResponseSelector::setCallback( Callback callback )
{
    mCallback = std::move( callback );
}

void ResponseSelector::setToggle( bool flag )
{
    mIsToggle = flag;
}

bool ResponseSelector::isSelectable( ) const
{
    return true;
}

void ResponseSelector::select( )
{
    Component::select( );

    mSprite.setFillColor( mSelected );
}

void ResponseSelector::deselect( )
{
    Component::deselect( );

    mSprite.setFillColor( mNormal );
}

void ResponseSelector::activate( )
{
    Component::activate( );

    // If we are toggle then we should show that the ResponseSelector is pressed and thus "toggled".
    if( mIsToggle )
        mSprite.setFillColor( mPressed );

    if( mCallback )
        mCallback( );

    // If we are not a toggle then deactivate the ResponseSelector since we are just momentarily activated.
    if( !mIsToggle )
        deactivate( );
}

void ResponseSelector::deactivate( )
{
    Component::deactivate( );

    if( mIsToggle )
    {
        // Reset texture to right one depending on if we are selected or not.
         if( isSelected( ) )
            mSprite.setFillColor( mSelected );
        else
            mSprite.setFillColor( mNormal );
    }
}

void ResponseSelector::handleEvent( const sf::Event& )
{
}

void ResponseSelector::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
    states.transform *= getTransform( );
    target.draw( mSprite, states );
    target.draw( mText, states );
}

}
