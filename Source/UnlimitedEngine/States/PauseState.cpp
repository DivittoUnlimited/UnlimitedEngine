#include "PauseState.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Game/DataTables.hpp"
#include "Core/Globals.hpp"
#include "Gui/Button.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

PauseState::PauseState(States::ID id, StateStack& stack, Context context , bool allowUpdates )
: State( id, stack, context )
, mPausedText( )
, mAllowUpdates( allowUpdates )
{
    sf::Font& font = context.fonts->get( FontMap.at( "Default" ) );
    sf::Vector2f viewSize = context.window->getView( ).getSize( );

    mPausedText.setFont( font );
    mPausedText.setString( "Paused" );
    mPausedText.setCharacterSize( 70 );
    centerOrigin( mPausedText );
    mPausedText.setPosition( 0.5f * viewSize.x, 0.2f * viewSize.y );

    auto ResumeGame = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    ResumeGame->setPosition( WINDOW_WIDTH / 2 - 100, 290 );
    ResumeGame->setText( "Resume" );
    ResumeGame->setCallback( [this] ( )
    {
        requestStackPop( );
    });

    auto settingsButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    settingsButton->setPosition( WINDOW_WIDTH / 2 - 100, 350 );
    settingsButton->setText( "Settings" );
    settingsButton->setCallback([this] ( )
    {
        requestStackPush( States::SettingsState );
    });

    auto exitButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    exitButton->setPosition( WINDOW_WIDTH / 2 - 100, 410 );
    exitButton->setText( "Return to Menu" );
    exitButton->setCallback([this] ( )
    {
        requestStateClear( );
        requestStackPush( States::Menu );
    });

    mGUIContainer.pack( ResumeGame );
    mGUIContainer.pack( settingsButton );
    mGUIContainer.pack( exitButton );
}

void PauseState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;
    window.setView(window.getDefaultView( ) );
    window.draw( mPausedText );
    window.draw( mGUIContainer );
}

bool PauseState::update( sf::Time )
{
    return mAllowUpdates;
}

bool PauseState::handleEvent( const sf::Event& event )
{
    if( event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape )
	{
		// Escape pressed, remove itself to return to the game
        requestStackPop( );
	}
    else
    {
        mGUIContainer.handleEvent( event );
    }
	return false;
}
