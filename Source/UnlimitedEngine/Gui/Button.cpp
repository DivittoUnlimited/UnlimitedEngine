#include "Button.hpp"
#include "Core/Utility.hpp"
#include "Core/DataTables.hpp"


#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace GUI
{

Button::Button( const FontManager& fonts, const TextureManager& textures )
: mCallback( )
, mTexture( textures.get( TextureMap.at( "Buttons" ) ) )
, mNormal(   0,   0, 200, 50 )
, mSelected( 0,  50, 200, 50 )
, mPressed(  0, 100, 200, 50 )
, mSprite()
, mText( "", fonts.get( FontMap.at( "Default" ) ), 16 )
, mIsToggle(false)
{
    mSprite.setTexture( mTexture );
    mSprite.setTextureRect( mNormal );

    sf::FloatRect bounds = mSprite.getLocalBounds( );
    mText.setPosition( bounds.width / 2.f, bounds.height / 2.f );
}

void Button::setCallback( Callback callback )
{
    mCallback = std::move( callback );
}

void Button::setText( const std::string& text )
{
    mText.setString( text );
    centerOrigin( mText );
}

void Button::setToggle( bool flag )
{
	mIsToggle = flag;
}

bool Button::isSelectable( ) const
{
    return true;
}

void Button::select( )
{
    Component::select( );

    mSprite.setTextureRect( mSelected );
}

void Button::deselect( )
{
    Component::deselect( );

    mSprite.setTextureRect( mNormal );
}

void Button::activate( )
{
    Component::activate( );

    // If we are toggle then we should show that the button is pressed and thus "toggled".
    if( mIsToggle )
        mSprite.setTextureRect( mPressed );

    if( mCallback )
        mCallback( );

    // If we are not a toggle then deactivate the button since we are just momentarily activated.
    if( !mIsToggle )
        deactivate( );
}

void Button::deactivate( )
{
    Component::deactivate( );

    if( mIsToggle )
	{
        // Reset texture to right one depending on if we are selected or not.
        if( isSelected( ) )
            mSprite.setTextureRect( mSelected );
		else
            mSprite.setTextureRect( mNormal );
	}
}

void Button::handleEvent( const sf::Event& )
{
}

void Button::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
    states.transform *= getTransform( );
    target.draw( mSprite, states );
    target.draw( mText, states );
}

}
