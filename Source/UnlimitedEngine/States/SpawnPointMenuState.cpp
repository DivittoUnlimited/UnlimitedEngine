#include "SpawnPointMenuState.hpp"
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

SpawnPointMenuState::SpawnPointMenuState( States::ID id, StateStack& stack, Context context, World* world )
: State( id, stack, context )
{
    mBackground = sf::RectangleShape( sf::Vector2f( 700, 600 ) );
    centerOrigin( mBackground );
    mBackground.setPosition( WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) );
    mBackground.setFillColor( sf::Color( 181, 182, 228, 200 ) );
    mBackground.setOutlineThickness( 3 );
    mBackground.setOutlineColor( sf::Color::Black );

    mText = sf::Text( );
    mText.setFont( context.fonts->get( FontMap.at( "Default" ) ) );
    mText.setString( "Who would you like\n\tto spawn?" );
    mText.setCharacterSize( 70 );
    centerOrigin( mText );
    mText.setPosition( WINDOW_WIDTH / 2, 200 );

    auto lightInfantry = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    lightInfantry->setPosition( WINDOW_WIDTH / 2 - 300, 300 );
    lightInfantry->setText( "Light Infantry" );
    lightInfantry->setCallback( [this, world] ( )
    {
        world->spawnUnit( UnitTypeMap.at( "LightInfantry" ), world->getSelectedBuilding( ) );
        this->requestStackPop( );
    });
    auto heavyInfantry = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    heavyInfantry->setPosition( WINDOW_WIDTH / 2 - 300, 400 );
    heavyInfantry->setText( "Heavy Infantry" );
    heavyInfantry->setCallback( [this] ( )
    {
        requestStackPop( );
    });
    auto phalanx = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    phalanx->setPosition( WINDOW_WIDTH / 2 - 300, 500 );
    phalanx->setText( "Phalanx" );
    phalanx->setCallback( [this] ( )
    {
        requestStackPop( );
    });

    auto none = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    none->setPosition( WINDOW_WIDTH / 2 + 125, 600 );
    none->setText( "Exit" );
    none->setCallback( [this] ( )
    {
        requestStackPop( );
    });

    mGUIContainer.pack( lightInfantry );
    mGUIContainer.pack( heavyInfantry );
    mGUIContainer.pack( phalanx );
    mGUIContainer.pack( none );
}

void SpawnPointMenuState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;
    window.setView(window.getDefaultView( ) );

    mWindow->draw( mBackground );
    mWindow->draw( mText );
    window.draw( mGUIContainer );
}

bool SpawnPointMenuState::update( sf::Time )
{
    return false;
}

bool SpawnPointMenuState::handleEvent( const sf::Event& event )
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
