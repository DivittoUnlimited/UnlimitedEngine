#include "TitleState.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Game/DataTables.hpp"
#include "Core/Globals.hpp"


#include <SFML/Graphics/RenderWindow.hpp>

TitleState::TitleState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mText( )
, mShowText( true )
, mTextEffectTime( sf::Time::Zero )
{
    mContext.textures->load( TextureMap.at( "DivittoUnlimitedLogo" ), MediaFileMap.at( "Textures" ).at( TextureMap.at( "DivittoUnlimitedLogo" ) ) );

    //mBackgroundSprite.setTexture( context.textures->get( TextureMap.at( "DivittoUnlimitedLogo" ) ) );

    mTitleText = sf::Text( "Tactics Tribes", mContext.fonts->get( FontMap.at( "Default" ) ), 100 );
    mTitleText.setFillColor( sf::Color( 181, 182, 228, 255 ) );
    centerOrigin( mTitleText );
    mTitleText.setPosition( WINDOW_WIDTH / 2, 200 );
    mTitleText.setOutlineColor( sf::Color( 79, 67, 174, 255 ) );
    mTitleText.setOutlineThickness( 5 );
    mTitleText.setStyle( sf::Text::Bold );

    mText.setFont( context.fonts->get( FontMap.at( "Default" ) ) );
    mText.setString( "PRESS START TO PLAY" );
    centerOrigin( mText );
    mText.setFillColor( sf::Color( 79, 67, 174, 255 ) );
    mText.setPosition( sf::Vector2f( context.window->getView( ).getCenter().x, context.window->getView( ).getCenter().y + (context.window->getView( ).getSize().y / 4) ) );
}

TitleState::~TitleState( )
{
    mContext.textures->remove( TextureMap.at( "DivittoUnlimitedLogo" ) );
}

void TitleState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;
    //window.draw( mBackgroundSprite );
    window.draw( mTitleText );
    if ( mShowText )
       window.draw( mText );
}

bool TitleState::update(sf::Time dt)
{
	mTextEffectTime += dt;

    if( mTextEffectTime >= sf::seconds( 0.5f ) )
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}

	return true;
}

bool TitleState::handleEvent( const sf::Event& event )
{
	// If any key is pressed, trigger the next screen
    if( event.type == sf::Event::KeyReleased || event.type == sf::Event::JoystickButtonPressed )
	{
        requestStackPop( );
        requestStackPush( States::Menu );
	}

	return true;
}
