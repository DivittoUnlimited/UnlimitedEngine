#include "EndTurnMenuState.hpp"

#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Game/DataTables.hpp"
#include "Core/Globals.hpp"
#include "Gui/Button.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

EndTurnMenuState::EndTurnMenuState(States::ID id, StateStack& stack, Context context, World* world )
    : State( id, stack, context )
    , mWorld( world )
{
    mChangeTurnText.setFont( context.fonts->get( FontMap.at( "Default" ) ) );
    mChangeTurnText.setString( "End your turn?" );
    mChangeTurnText.setCharacterSize( 70 );
    centerOrigin( mChangeTurnText );
    mChangeTurnText.setPosition( WINDOW_WIDTH / 2, 200 );

    mChangeTurnBackground = sf::RectangleShape( sf::Vector2f( 500, 125 ) );
    centerOrigin( mChangeTurnBackground );
    mChangeTurnBackground.setPosition( WINDOW_WIDTH / 2, 200  );
    mChangeTurnBackground.setFillColor( sf::Color( 181, 182, 228, 255 ) );
    mChangeTurnBackground.setOutlineThickness( 3 );
    mChangeTurnBackground.setOutlineColor( sf::Color::Black );

    auto yesButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    yesButton->setPosition( WINDOW_WIDTH / 2 - 100, 390 );
    yesButton->setText( "YES" );
    yesButton->setCallback( [this] ( )
    {
        if( mWorld->mNetworkedWorld )
            mWorld->mNetworkNode->notifyGameAction( GameActions::ChangeTurn, sf::Vector2f( 0, 0 ) );
        else
        {
            mWorld->changeTurn();
        }
        mWorld->mStateStack->popState();
    });
    auto noButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    noButton->setPosition( WINDOW_WIDTH / 2 - 100, 450 );
    noButton->setText( "NO" );
    noButton->setCallback( [this] ( )
    {
        mWorld->mStateStack->popState();
    });
    mChangeTurnGUI.pack( yesButton );
    mChangeTurnGUI.pack( noButton );
}

void EndTurnMenuState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;
    window.setView(window.getDefaultView( ) );

    window.draw( mChangeTurnBackground );
    window.draw( mChangeTurnText );
    window.draw( mChangeTurnGUI );
}

bool EndTurnMenuState::update( sf::Time )
{
    return false;
}

bool EndTurnMenuState::handleEvent( const sf::Event& event )
{
    if( event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape )
    {
        // Escape pressed, remove itself to return to the game
        requestStackPop( );
    }
    else
        mChangeTurnGUI.handleEvent( event );

    return false;
}

