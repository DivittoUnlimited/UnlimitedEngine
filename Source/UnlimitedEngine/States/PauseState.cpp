#include "PauseState.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/DataTables.hpp"


#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

PauseState::PauseState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mBackgroundSprite( )
, mPausedText( )
, mInstructionText( )
{
    sf::Font& font = context.fonts->get( FontMap.at( "Default" ) );
    sf::Vector2f viewSize = context.window->getView( ).getSize( );

    mPausedText.setFont( font );
    mPausedText.setString( "Paused" );
    mPausedText.setCharacterSize( 70 );
    centerOrigin( mPausedText );
    mPausedText.setPosition( 0.5f * viewSize.x, 0.4f * viewSize.y );

    mInstructionText.setFont( font );
    mInstructionText.setString( "(Press Backspace/Start to return to the main menu)" );
    centerOrigin( mInstructionText );
    mInstructionText.setPosition( 0.5f * viewSize.x, 0.6f * viewSize.y );
}

void PauseState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;
    window.setView(window.getDefaultView( ) );

	sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor( sf::Color( 0, 0, 0, 150 ) );
    backgroundShape.setSize( window.getView( ).getSize( ) );

    window.draw( backgroundShape );
    window.draw( mPausedText );
    window.draw( mInstructionText );
}

bool PauseState::update( sf::Time )
{
	return false;
}

bool PauseState::handleEvent( const sf::Event& event )
{
    if( event.type != sf::Event::KeyPressed )
		return false;

    if( event.key.code == sf::Keyboard::Escape )
	{
		// Escape pressed, remove itself to return to the game
        requestStackPop( );
	}

    ///
    /// REAAD ME!!!
    ///
    // NEED TO ADD BOOLEAN TO KNOW IF START BUTTON SHOULD PAUSE OR UN-PAUSE

    if( ( event.type == sf::Event::JoystickButtonReleased && event.joystickButton.button == 9 ) || ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Backspace ) )
	{
		// Escape pressed, remove itself to return to the game
        requestStateClear( );
        requestStackPush( States::Menu );
	}

	return false;
}
