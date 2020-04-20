#include "ActionMenuState.hpp"

#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Game/DataTables.hpp"
#include "Core/Globals.hpp"
#include "Gui/Button.hpp"
#include "Game/World.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

ActionMenuState::ActionMenuState( States::ID id, StateStack& stack, Context context, World* world )
    : State( id, stack, context )
{
    mBackground = sf::RectangleShape( sf::Vector2f( WINDOW_WIDTH - 100, WINDOW_HEIGHT / 3) );
    mBackground.setPosition( 50, (WINDOW_HEIGHT / 2) + 100 );
    mBackground.setFillColor( sf::Color( 181, 182, 228, 200 ) );
    mBackground.setOutlineThickness( 3 );
    mBackground.setOutlineColor( sf::Color::Black );

    auto Move = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    Move->setPosition( mBackground.getPosition().x + 50, mBackground.getPosition().y + 50 );
    Move->setText( "Move" );
    Move->setCallback( [this, world] ( )
    {
        // Call Grid::moveSelectedUnit
        this->requestStackPop( );
    });

    auto Actions = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    Actions->setPosition( mBackground.getPosition().x + mBackground.getSize().x - 50, mBackground.getPosition().y + 50 );
    Actions->setText( "Actions" );
    Actions->setCallback( [this, world] ( )
    {
        // Call Actions menu
        this->requestStackPop( );
    });

    auto Wait = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    Wait->setPosition( mBackground.getPosition().x + 50, mBackground.getPosition().y + mBackground.getSize().y - 50 );
    Wait->setText( "Wait" );
    Wait->setCallback( [this, world] ( )
    {
        // Set unit hasMoved, and hasSpentAction to true
        // OPTIONAL Call wait menu
            // Set stance see notes
        this->requestStackPop( );
    });

    auto none = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    none->setPosition( mBackground.getPosition().x + 50, mBackground.getPosition().y + mBackground.getSize().y - 50 );
    none->setText( "Exit" );
    none->setCallback( [this] ( )
    {
        requestStackPop( );
    });

    mGUIContainer.pack( Move );
    mGUIContainer.pack( Actions );
    mGUIContainer.pack( Wait );
    mGUIContainer.pack( none );
}

void ActionMenuState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;
    window.setView(window.getDefaultView( ) );

    mWindow->draw( mBackground );
    mWindow->draw( mText );
    window.draw( mGUIContainer );
}

bool ActionMenuState::update( sf::Time )
{
    return false;
}

bool ActionMenuState::handleEvent( const sf::Event& event )
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
