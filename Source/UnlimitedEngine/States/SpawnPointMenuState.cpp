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

    auto fighter = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    fighter->setPosition( WINDOW_WIDTH / 2 - 300, 300 );
    fighter->setText( "Fighter" );
    fighter->setCallback( [this, world] ( )
    {
        this->requestStackPop( );
    });

    auto templar = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    templar->setPosition( WINDOW_WIDTH / 2 - 300, 360 );
    templar->setText( "Templar" );
    templar->setCallback( [this, world] ( )
    {
        this->requestStackPop( );
    });

    auto medic = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    medic->setPosition( WINDOW_WIDTH / 2 - 300, 420 );
    medic->setText( "Medic" );
    medic->setCallback( [this, world] ( )
    {
        this->requestStackPop( );
    });

    auto rogue = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    rogue->setPosition( WINDOW_WIDTH / 2 - 300, 480 );
    rogue->setText( "Rogue" );
    rogue->setCallback( [this, world] ( )
    {
        this->requestStackPop( );
    });

    auto archer = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    archer->setPosition( WINDOW_WIDTH / 2, 300 );
    archer->setText( "Archer" );
    archer->setCallback( [this, world] ( )
    {
        this->requestStackPop( );
    });

    auto scout = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    scout->setPosition( WINDOW_WIDTH / 2, 360 );
    scout->setText( "Scout" );
    scout->setCallback( [this, world] ( )
    {
        this->requestStackPop( );
    });

    auto wizard = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    wizard->setPosition( WINDOW_WIDTH / 2, 420 );
    wizard->setText( "Wizard" );
    wizard->setCallback( [this, world] ( )
    {
        this->requestStackPop( );
    });

    auto bard = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    bard->setPosition( WINDOW_WIDTH / 2, 480 );
    bard->setText( "Bard" );
    bard->setCallback( [this, world] ( )
    {
        this->requestStackPop( );
    });

    auto none = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    none->setPosition( WINDOW_WIDTH / 2 + 125, 600 );
    none->setText( "Exit" );
    none->setCallback( [this] ( )
    {
        requestStackPop( );
    });

    mGUIContainer.pack( fighter );
    mGUIContainer.pack( templar );
    mGUIContainer.pack( medic );
    mGUIContainer.pack( rogue );
    mGUIContainer.pack( archer );
    mGUIContainer.pack( scout );
    mGUIContainer.pack( wizard );
    mGUIContainer.pack( bard );
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
