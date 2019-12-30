#include "PauseState.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/DataTables.hpp"
#include "Core/Globals.hpp"
#include "Gui/Button.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

PauseState::PauseState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mPausedText( )
{
    sf::Font& font = context.fonts->get( FontMap.at( "Default" ) );
    sf::Vector2f viewSize = context.window->getView( ).getSize( );

    mPausedText.setFont( font );
    mPausedText.setString( "Paused" );
    mPausedText.setCharacterSize( 70 );
    centerOrigin( mPausedText );
    mPausedText.setPosition( 0.5f * viewSize.x, 0.2f * viewSize.y );

    auto ResumeGame = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    ResumeGame->setPosition( WINDOW_WIDTH / 2 - 100, 390 );
    ResumeGame->setText( "Resume" );
    ResumeGame->setCallback( [this] ( )
    {
        requestStackPop( );
    });

    auto playerWins = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    playerWins->setPosition( WINDOW_WIDTH / 2 - 100, 450 );
    playerWins->setText( "Player Wins!" );
    playerWins->setCallback( [this] ( )
    {
        //requestStackPop( );
        //requestStackPush( States::SinglePlayerMenuState );
    });

    auto playerLoses = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    playerLoses->setPosition( WINDOW_WIDTH / 2 - 100, 510 );
    playerLoses->setText( "Player Loses!" );
    playerLoses->setCallback( [this] ( )
    {
        //requestStackPop( );
        //requestStackPush( States::HostGame );
    });

    auto settingsButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    settingsButton->setPosition( WINDOW_WIDTH / 2 - 100, 570 );
    settingsButton->setText( "Settings" );
    settingsButton->setCallback([this] ( )
    {
        requestStackPush( States::SettingsState );
    });

    auto exitButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    exitButton->setPosition( WINDOW_WIDTH / 2 - 100, 630 );
    exitButton->setText( "Return to Menu" );
    exitButton->setCallback([this] ( )
    {
        requestStateClear( );
        requestStackPush( States::Menu );
    });

    mGUIContainer.pack( ResumeGame );
    mGUIContainer.pack( playerWins );
    mGUIContainer.pack( playerLoses );
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
	return false;
}

bool PauseState::handleEvent( const sf::Event& event )
{
    if( event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape )
	{
		// Escape pressed, remove itself to return to the game
        requestStackPop( );
	}
    else
        mGUIContainer.handleEvent( event );

	return false;
}
